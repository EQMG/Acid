#include "RendererTest.hpp"

namespace Flounder
{
	const std::vector<Vertex> triangleVertices =
	{
		{ Vector3(-0.5f, -0.5f, 0.0f), Colour(1.0f, 0.0f, 0.0f) },
		{ Vector3(0.5f, -0.5f, 0.0f), Colour(0.0f, 1.0f, 0.0f) },
		{ Vector3(0.5f, 0.5f, 0.0f), Colour(0.0f, 0.0f, 1.0f) },
		{ Vector3(-0.5f, 0.5f, 0.0f), Colour(1.0f, 1.0f, 1.0f) }
	};
	const std::vector<uint16_t> triangleIndices =
	{
		0, 1, 2, 2, 3, 0
	};

	RendererTest::RendererTest() :
		IRenderer(),
		m_shader(Shader("tests", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		)),
		m_pipeline(Pipeline("tests", PipelinePolygon, m_shader)),
		m_vertexBuffer(VertexBuffer(triangleVertices)),
		m_indexBuffer(IndexBuffer(triangleIndices))
	{
		const auto logicalDevice = Display::Get()->GetDevice();
		const auto physicalDevice = Display::Get()->GetPhysicalDevice();
		const auto surface = Display::Get()->GetSurface();
		const auto queue = Display::Get()->GetQueue();
		const auto renderPass = Renderer::Get()->GetRenderPass();
		const auto commandPool = Renderer::Get()->GetCommandPool();

		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescriptions = Vertex::GetAttributeDescriptions();
		VertexInputState vertexInputState = {};
		vertexInputState.bindingDescriptionCount = 1;
		vertexInputState.attributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();
		
		m_shader.Create(logicalDevice);
		m_pipeline.Create(logicalDevice, renderPass, vertexInputState);
		m_vertexBuffer.Create(logicalDevice, physicalDevice, surface, queue, commandPool);
		m_indexBuffer.Create(logicalDevice, physicalDevice, surface, queue, commandPool);
	}

	RendererTest::~RendererTest()
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		m_shader.Cleanup(logicalDevice);
		m_pipeline.Cleanup(logicalDevice);
		m_vertexBuffer.Cleanup(logicalDevice);
		m_indexBuffer.Cleanup(logicalDevice);
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipeline());

		VkBuffer vertexBuffers[] = { m_vertexBuffer.GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(*commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(*commandBuffer, m_indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*commandBuffer, m_indexBuffer.GetIndicesSize(), 1, 0, 0, 0);

	/*	AllocatedUniform uboAllocated;
		UboTest *ubo = reinterpret_cast<UboTest*>(m_shader->AllocateUniform(sizeof(UboTest), &uboAllocated));
		ubo->memes = true;
		ubo->projection = Matrix4();

		VkDescriptorSet descriptorSets[1] = { uboAllocated.descriptorSet };
	//	uint32_t bufferOffsets[1] = { uboAllocated.bufferOffset }; // TODO: Use!
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptorSets, 1, &uboAllocated.bufferOffset);*/
	}
}
