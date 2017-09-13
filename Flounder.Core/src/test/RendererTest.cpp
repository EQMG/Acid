#include "RendererTest.hpp"

namespace Flounder
{
	RendererTest::RendererTest(CommandPool *commandPoolTransfer)
	{
		m_shader = new Shader("tests", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		);
		const std::vector<Vertex> verticesTriangle =
		{
			{ Vector3(0.0f, -0.5f, 0.0f), Colour(1.0f, 0.0f, 0.0f) },
			{ Vector3(0.5f, 0.5f, 0.0f), Colour(0.0f, 1.0f, 0.0f) },
			{ Vector3(-0.5f, 0.5f, 0.0f), Colour(0.0f, 0.0f, 1.0f) }
		};
		m_vertexBuffer = new VertexBuffer(verticesTriangle);

		const std::vector<Vertex> verticesTriangle1 =
		{
			{ Vector3(0.5f, -0.5f, 0.0f), Colour(0.0f, 1.0f, 0.0f) },
			{ Vector3(1.0f, 0.5f, 0.0f), Colour(0.0f, 0.0f, 1.0f) },
			{ Vector3(0.0f, 0.5f, 0.0f), Colour(1.0f, 0.0f, 0.0f) }
		};
		m_vertexBuffer1 = new VertexBuffer(verticesTriangle1);

		VkDevice logicalDevice = Display::Get()->GetVkDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetVkPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetVkSurface();
		VkQueue transferQueue = Display::Get()->GetVkTransferQueue();
	//	CommandPool *commandPoolTransfer = Renderer::Get()->GetCommandPoolTransfer();

		m_shader->Create(&logicalDevice);
		m_vertexBuffer->Create(&logicalDevice, &physicalDevice, &surface, commandPoolTransfer->GetCommandPool(), &transferQueue);
		m_vertexBuffer1->Create(&logicalDevice, &physicalDevice, &surface, commandPoolTransfer->GetCommandPool(), &transferQueue);
	}

	RendererTest::~RendererTest()
	{
		VkDevice logicalDevice = Display::Get()->GetVkDevice();

		m_shader->Cleanup(&logicalDevice);
		delete m_shader;

		m_vertexBuffer->Cleanup(&logicalDevice);
		delete m_vertexBuffer;

		m_vertexBuffer1->Cleanup(&logicalDevice);
		delete m_vertexBuffer1;
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
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(m_vertexBuffer->GetVerticesSize()), 1, 0, 0);

		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		VkBuffer vertexBuffers1[] = { *m_vertexBuffer1->GetBuffer() };
		VkDeviceSize offsets1[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers1, offsets1);
		vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(m_vertexBuffer1->GetVerticesSize()), 1, 0, 0);
	}

	void RendererTest::Render(const Vector4 &clipPlane, const ICamera &camera)
	{
	}
}
