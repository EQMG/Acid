#include "PipelineMaterial.hpp"

#include "Renderer/Renderer.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Pipeline::Stage& pipelineStage, const PipelineGraphicsCreate& pipelineCreate)
{
	auto temp = PipelineMaterial(pipelineStage, pipelineCreate);
	Metadata metadata = Metadata();
	temp.Encode(metadata);

	auto resource = Resources::Get()->Find(metadata);

	if(resource != nullptr)
		{
			return std::dynamic_pointer_cast<PipelineMaterial>(resource);
		}

	auto result = std::make_shared<PipelineMaterial>(pipelineStage, pipelineCreate);
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	//	result->Decode(metadata);
	//	result->Load();
	return result;
}

PipelineMaterial::PipelineMaterial(const Pipeline::Stage& pipelineStage, const PipelineGraphicsCreate& pipelineCreate) : m_pipelineStage(pipelineStage), m_pipelineCreate(pipelineCreate), m_renderStage(nullptr), m_pipeline(nullptr) {}

bool PipelineMaterial::BindPipeline(const CommandBuffer& commandBuffer)
{
	auto renderStage = Renderer::Get()->GetRenderStage(m_pipelineStage.first);

	if(renderStage == nullptr)
		{
			return false;
		}
	if(m_renderStage != renderStage)
		{
			m_renderStage = renderStage;
			m_pipeline.reset(m_pipelineCreate.Create(m_pipelineStage));
		}

	m_pipeline->BindPipeline(commandBuffer);
	return true;
}

void PipelineMaterial::Decode(const Metadata& metadata)
{
	metadata.GetChild("Renderpass", m_pipelineStage.first);
	metadata.GetChild("Subpass", m_pipelineStage.second);
	metadata.GetChild("Pipeline Create", m_pipelineCreate);
}

void PipelineMaterial::Encode(Metadata& metadata) const
{
	metadata.SetChild("Renderpass", m_pipelineStage.first);
	metadata.SetChild("Subpass", m_pipelineStage.second);
	metadata.SetChild("Pipeline Create", m_pipelineCreate);
}
}
