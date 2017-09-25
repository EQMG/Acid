#include "RendererTest.hpp"

namespace Flounder
{
	RendererTest::RendererTest(VkRenderPass renderPass, CommandPool *commandPoolTransfer)
	{
		m_shader = new Shader("tests", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		);

		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();
		VertexInputState vertexInputState = {};
		vertexInputState.bindingDescriptionCount = 1;
		vertexInputState.attributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();
		m_pipeline = new Pipeline("tests", PipelinePolygon, vertexInputState, m_shader);
		
		const std::vector<Vertex> triangleVertices =
		{
			{ Vector3(-0.5f, -0.5f, 0.0f), Colour(1.0f, 0.0f, 0.0f) },
			{ Vector3(0.5f, -0.5f, 0.0f), Colour(0.0f, 1.0f, 0.0f) },
			{ Vector3(0.5f, 0.5f, 0.0f), Colour(0.0f, 0.0f, 1.0f) },
			{ Vector3(-0.5f, 0.5f, 0.0f), Colour(1.0f, 1.0f, 1.0f) }
		};
		const std::vector<uint16_t> triangleIndices = {
			0, 1, 2, 2, 3, 0
		};
		m_vertexBuffer = new VertexBuffer(triangleVertices);
		m_indexBuffer = new IndexBuffer(triangleIndices);

		VkDevice logicalDevice = Display::Get()->GetDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetSurface();
		VkQueue queue = Display::Get()->GetQueue();
		VkCommandPool *commandPool = commandPoolTransfer->GetCommandPool();

		m_shader->Create(&logicalDevice);
		m_pipeline->Create(&logicalDevice, renderPass);
		m_vertexBuffer->Create(&logicalDevice, &physicalDevice, &surface, commandPool, &queue);
		m_indexBuffer->Create(&logicalDevice, &physicalDevice, &surface, commandPool, &queue);
	}

	RendererTest::~RendererTest()
	{
		VkDevice logicalDevice = Display::Get()->GetDevice();

		m_shader->Cleanup(&logicalDevice);
		delete m_shader;

		m_pipeline->Cleanup(&logicalDevice);
		delete m_pipeline;

		m_vertexBuffer->Cleanup(&logicalDevice);
		delete m_vertexBuffer;

		m_indexBuffer->Cleanup(&logicalDevice);
		delete m_indexBuffer;
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(*commandBuffer, 0, 1, m_vertexBuffer->GetBuffer(), offsets);
		vkCmdBindIndexBuffer(*commandBuffer, *m_indexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*commandBuffer, m_indexBuffer->GetIndicesSize(), 1, 0, 0, 0);

	/*	AllocatedUniform uboAllocated;
		UboTest *ubo = reinterpret_cast<UboTest*>(m_shader->AllocateUniform(sizeof(UboTest), &uboAllocated));
		ubo->memes = true;
		ubo->projection = Matrix4();

		VkDescriptorSet descriptorSets[1] = { uboAllocated.descriptorSet };
	//	uint32_t bufferOffsets[1] = { uboAllocated.bufferOffset }; // TODO: Use!
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptorSets, 1, &uboAllocated.bufferOffset);*/
	}
}
