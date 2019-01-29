#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Metadata &metadata)
	{
		std::shared_ptr<Resource> resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<PipelineMaterial>(resource);
		}

		Pipeline::Stage pipelineStage = {};
		pipelineStage.first = metadata.GetChild<uint32_t>("Renderpass");
		pipelineStage.second = metadata.GetChild<uint32_t>("Subpass");
		PipelineGraphicsCreate pipelineCreate = PipelineGraphicsCreate();
		auto result = std::make_shared<PipelineMaterial>(pipelineStage, pipelineCreate);
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate)
	{
	//	Metadata metadata = Metadata();
	//	metadata.SetChild<std::string>("Filename", filename);
	//	metadata.SetChild<std::string>("Style", style);
	//	return Create(metadata);

		return std::make_shared<PipelineMaterial>(pipelineStage, pipelineCreate);
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

	void PipelineMaterial::Encode(Metadata &metadata) const
	{
		metadata.SetChild<uint32_t>("Renderpass", m_pipelineStage.first);
		metadata.SetChild<uint32_t>("Subpass", m_pipelineStage.second);

		auto shadersNode = metadata.FindChild("Shaders", false);
		auto definesNode = metadata.FindChild("Defines", false);

		if (shadersNode == nullptr)
		{
			shadersNode = metadata.AddChild(new Metadata("Shaders"));
		}

		if (definesNode == nullptr)
		{
			definesNode = metadata.AddChild(new Metadata("Defines"));
		}

		for (const auto &shaderStage : m_pipelineCreate.GetShaderStages())
		{
			shadersNode->AddChild(new Metadata(shaderStage));
		}

		for (const auto &define : m_pipelineCreate.GetDefines())
		{
			definesNode->AddChild(new Metadata(define.first, define.second));
		}
	}
}
