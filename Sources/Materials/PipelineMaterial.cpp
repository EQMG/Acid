#include "PipelineMaterial.hpp"

#include <numeric>

namespace acid
{
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
		result << "Material_";
		result << std::to_string(graphicsStage.GetRenderpass()) << "_";
		result << std::to_string(graphicsStage.GetSubpass()) << "_";

		for (auto &stage : pipelineCreate.GetShaderStages())
		{
			result << stage << "_";
		}

		for (auto &element : pipelineCreate.GetDefines())
		{
			result << element.GetName() << "=";
			result << element.GetValue() << "_";
		}

		return result.str();
	}
}
