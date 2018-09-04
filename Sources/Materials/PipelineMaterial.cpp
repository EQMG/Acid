#include "PipelineMaterial.hpp"

#include "Resources/Resources.hpp"

namespace acid
{
	std::shared_ptr<PipelineMaterial> PipelineMaterial::Resource(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate)
	{
		auto resource = Resources::Get()->Get(ToFilename(graphicsStage, pipelineCreate));

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<PipelineMaterial>(resource);
		}

		auto result = std::make_shared<PipelineMaterial>(graphicsStage, pipelineCreate);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	PipelineMaterial::PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate) :
		m_filename(ToFilename(graphicsStage, pipelineCreate)),
		m_pipeline(Pipeline(graphicsStage, pipelineCreate))
	{
	}

	PipelineMaterial::~PipelineMaterial()
	{
	}

	std::string PipelineMaterial::ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate)
	{
		std::stringstream result;
		result << "Material_" << graphicsStage.GetRenderpass() << "_" << graphicsStage.GetSubpass() << "_";

		for (auto &stage : pipelineCreate.GetShaderStages())
		{
			result << stage << "_";
		}

		for (auto &element : pipelineCreate.GetDefines())
		{
			result << element.GetName() << "=" << element.GetValue() << "_";
		}

		return result.str();
	}
}
