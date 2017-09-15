#include "RendererTest.hpp"

namespace Flounder
{
	RendererTest::RendererTest(CommandPool *commandPoolTransfer)
	{
		m_shader = new Shader("tests", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		);
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

		VkDevice logicalDevice = Display::Get()->GetVkDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetVkPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetVkSurface();
		VkQueue transferQueue = Display::Get()->GetVkTransferQueue();
	//	CommandPool *commandPoolTransfer = Renderer::Get()->GetCommandPoolTransfer();

		m_shader->Create(&logicalDevice);
		m_vertexBuffer->Create(&logicalDevice, &physicalDevice, &surface, commandPoolTransfer->GetCommandPool(), &transferQueue);
		m_indexBuffer->Create(&logicalDevice, &physicalDevice, &surface, commandPoolTransfer->GetCommandPool(), &transferQueue);
	}

	RendererTest::~RendererTest()
	{
		VkDevice logicalDevice = Display::Get()->GetVkDevice();

		m_shader->Cleanup(&logicalDevice);
		delete m_shader;

		m_vertexBuffer->Cleanup(&logicalDevice);
		delete m_vertexBuffer;

		m_indexBuffer->Cleanup(&logicalDevice);
		delete m_indexBuffer;
	}

	void RendererTest::CreatePipeline(std::vector<VkPipelineShaderStageCreateInfo> *shaderStages)
	{
		for (auto stage : *m_shader->GetStages())
		{
			shaderStages->push_back(stage);
		}
	}

	void RendererTest::CreateCommands(size_t i, std::vector<VkCommandBuffer> commandBuffers, VkPipeline graphicsPipeline)
	{
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		VkBuffer vertexBuffers[] = { *m_vertexBuffer->GetBuffer() };
		VkBuffer indexBuffers[] = { *m_indexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffers[i], indexBuffers[0], 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(m_indexBuffer->GetIndicesSize()), 1, 0, 0, 0);
	}

	void RendererTest::Render(const Vector4 &clipPlane, const ICamera &camera)
	{
	}
}
