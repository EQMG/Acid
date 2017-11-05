#include "RendererDeferred.hpp"

#include "../devices/Display.hpp"
#include "../models/Vertex.hpp"
#include "../renderer/Renderer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	const std::vector<Vertex> VERTICES = {
		Vertex(Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f))
	};
	const std::vector<uint32_t> INDICES = {
		0, 1, 2, 2, 3, 0
	};

	const DescriptorType RendererDeferred::typeSamplerColour = Texture::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType RendererDeferred::typeSamplerNormal = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);
	const PipelineCreateInfo RendererDeferred::pipelineCreateInfo =
	{
		PIPELINE_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_NONE, // cullModeFlags
		1, // subpass

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{ typeSamplerColour, typeSamplerNormal }, // descriptors

		{ "res/shaders/deferred/deferred.vert.spv", "res/shaders/deferred/deferred.frag.spv" } // shaderStages
	};

	RendererDeferred::RendererDeferred() :
		IRenderer(),
		m_pipeline(new Pipeline("deferred", pipelineCreateInfo)),
		m_model(new Model(VERTICES, INDICES))
	{
	}

	RendererDeferred::~RendererDeferred()
	{
		delete m_model;
		delete m_pipeline;
	}

	void RendererDeferred::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(0, descriptorSet), Renderer::Get()->GetSwapchain()->GetNormalImage()->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { m_pipeline->GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(*commandBuffer);
	}
}
