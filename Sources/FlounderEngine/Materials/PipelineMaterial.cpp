#include "PipelineMaterial.hpp"

#include <numeric>

namespace Flounder
{
	PipelineMaterial::PipelineMaterial(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<Define> &defines) :
		m_filename(ToFilename(graphicsStage, pipelineCreate, defines)),
		m_pipeline(new Pipeline(graphicsStage, pipelineCreate, defines))
	{
	}

	PipelineMaterial::~PipelineMaterial()
	{
		delete m_pipeline;
	}

	void PipelineMaterial::CmdRender(const VkCommandBuffer &commandBuffer, Model *model, DescriptorSet *descriptorSet)
	{
		m_pipeline->BindPipeline(commandBuffer);
		descriptorSet->BindDescriptor(commandBuffer);
		model->CmdRender(commandBuffer, 1);
	}

	std::string PipelineMaterial::ToFilename(const GraphicsStage &graphicsStage, const PipelineCreate &pipelineCreate, const std::vector<Define> &defines)
	{
		std::string shaderString;
		shaderString = std::accumulate(std::begin(pipelineCreate.m_shaderStages), std::end(pipelineCreate.m_shaderStages), shaderString);

		//	std::string defineString;
		//	auto defineLambda = [](Define a, Define b){a.name + std::to_string(a.value) + b.name + std::to_string(b.value); };
		//	defineString = std::accumulate(std::begin(defines), std::end(defines), defineString, defineLambda);
		std::string defineString;
		for (auto element : defines)
		{ defineString += element.name + element.value + "_"; }

		return "Material_" + std::to_string(graphicsStage.renderpass) + "_" + std::to_string(graphicsStage.subpass) + "_" + shaderString + "_" + defineString;
	}
}
