#include "PipelineMaterial.hpp"

#include <numeric>

namespace fl
{
	PipelineMaterial::PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<PipelineDefine> &defines) :
		m_filename(ToFilename(graphicsStage, pipelineCreate, defines)),
		m_pipeline(new Pipeline(graphicsStage, pipelineCreate, defines)),
		m_uniformScene(new UniformHandler())
	{
	}

	PipelineMaterial::~PipelineMaterial()
	{
		delete m_pipeline;
		delete m_uniformScene;
	}

	std::string PipelineMaterial::ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<PipelineDefine> &defines)
	{
		std::string shaderString;

		for (auto stage : pipelineCreate.GetShaderStages())
		{
			shaderString += stage + "_";
		}

		std::string defineString;

		for (auto element : defines)
		{
			defineString += element.GetName() + element.GetValue() + "_";
		}

		return "Material_" + std::to_string(graphicsStage.GetRenderpass()) + "_" + std::to_string(graphicsStage.GetSubpass()) + "_" + shaderString + "_" + defineString;
	}
}
