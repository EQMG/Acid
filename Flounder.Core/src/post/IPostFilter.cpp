#include "IPostFilter.hpp"

#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	const std::vector<Vertex> VERTICES = 
	{
		Vertex(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f))
	};
	const std::vector<uint32_t> INDICES = 
	{
		0, 3, 2, 2, 1, 0
	};

	const DescriptorType IPostFilter::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType IPostFilter::typeSamplerColour = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo IPostFilter::pipelineCreateInfo =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeUboScene, typeSamplerColour }, // descriptors

		{ "res/shaders/filters/default.vert.spv", "res/shaders/filters/default.frag.spv" } // shaderStages
	};

	IPostFilter::IPostFilter(const std::string &filterName, const std::string &fragmentShader, const int &subpass) :
		m_pipeline(nullptr),
		m_model(new Model(VERTICES, INDICES))
	{
		PipelineCreateInfo pipelineCreateInfo = PipelineCreateInfo(this->pipelineCreateInfo);
		pipelineCreateInfo.shaderStages[1] = fragmentShader; // fragment
		m_pipeline = new Pipeline(filterName, pipelineCreateInfo, subpass);
	}

	IPostFilter::~IPostFilter()
	{
		delete m_model;
		delete m_pipeline;
	}

	void IPostFilter::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}
}
