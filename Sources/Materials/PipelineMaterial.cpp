#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	std::shared_ptr<PipelineMaterial> PipelineMaterial::Create(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate)
	{
		auto resource = Resources::Get()->Find(ToName(pipelineStage, pipelineCreate));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<PipelineMaterial>(resource);
		}

		auto result = std::make_shared<PipelineMaterial>(pipelineStage, pipelineCreate);
		Resources::Get()->Add(std::dynamic_pointer_cast<Resource>(result));
		return result;
	}

	PipelineMaterial::PipelineMaterial(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate) :
		Resource(ToName(pipelineStage, pipelineCreate)),
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

	std::string PipelineMaterial::ToName(const Pipeline::Stage &pipelineStage, const PipelineGraphicsCreate &pipelineCreate)
	{
		std::stringstream result;
		result << "Material_" << pipelineStage.first << "_" << pipelineStage.second << "_";

		for (const auto &stage : pipelineCreate.GetShaderStages())
		{
			result << stage << "_";
		}

		for (const auto &element : pipelineCreate.GetDefines())
		{
			result << element.first << "=" << element.second << "_";
		}

		return result.str();
	}
}
