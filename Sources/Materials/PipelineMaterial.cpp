#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Metadata &metadata)
	{
		auto result = std::make_shared<PipelineMaterial>(Pipeline::Stage(0, 0), PipelineGraphicsCreate());
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate)
	{
		return std::make_shared<PipelineMaterial>(pipelineStage, pipelineCreate);
		/*auto temp = PipelineMaterial(pipelineStage, pipelineCreate);
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);*/
	}

	PipelineMaterial::PipelineMaterial(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate) :
		m_pipelineStage(pipelineStage),
		m_pipelineCreate(pipelineCreate),
		m_renderStage(nullptr),
		m_pipeline(nullptr)
	{
	}

	bool PipelineMaterial::BindPipeline(const CommandBuffer &commandBuffer)
	{
		auto renderStage = Renderer::Get()->GetRenderStage(m_pipelineStage.first);

		if (renderStage == nullptr)
		{
			return false;
		}
		else if (m_renderStage != renderStage)
		{
			m_renderStage = renderStage;
			m_pipeline.reset(m_pipelineCreate.Create(m_pipelineStage));
		}

		m_pipeline->BindPipeline(commandBuffer);
		return true;
	}

	void PipelineMaterial::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Renderpass", m_pipelineStage.first);
		metadata.GetChild("Subpass", m_pipelineStage.second);
		metadata.GetChild("Pipeline Create", m_pipelineCreate);
	}

	void PipelineMaterial::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Renderpass", m_pipelineStage.first);
		metadata.SetChild("Subpass", m_pipelineStage.second);
		metadata.SetChild("Pipeline Create", m_pipelineCreate);
	}
}
