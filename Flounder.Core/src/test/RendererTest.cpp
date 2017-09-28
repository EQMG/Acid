#include "RendererTest.hpp"

namespace Flounder
{
	const std::vector<Vertex> triangleVertices = 
	{
		{ Vector3(-0.5f, -0.5f,  0.5f), Colour(0.0f, 0.0f, 1.0f) },
		{ Vector3(-0.5f,  0.5f,  0.5f), Colour(1.0f, 0.0f, 0.0f) },
		{ Vector3(0.5f,  0.5f,  0.5f), Colour(0.0f, 1.0f, 0.0f) },
		{ Vector3(0.5f, -0.5f,  0.5f), Colour(1.0f, 1.0f, 0.0f) },
		{ Vector3(-0.5f, -0.5f, -0.5f), Colour(1.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f,  0.5f, -0.5f), Colour(1.0f, 0.0f, 0.0f) },
		{ Vector3(0.5f,  0.5f, -0.5f), Colour(1.0f, 0.0f, 1.0f) },
		{ Vector3(0.5f, -0.5f, -0.5f), Colour(0.0f, 0.0f, 1.0f) }
	};
	const std::vector<uint16_t> triangleIndices =
	{
		0, 2, 1,  0, 3, 2,
		4, 3, 0,  4, 7, 3,
		4, 1, 5,  4, 0, 1,
		3, 6, 2,  3, 7, 6,
		1, 6, 5,  1, 2, 6,
		7, 5, 6,  7, 4, 5
	};

	RendererTest::RendererTest() :
		IRenderer(),
		m_cameraUniformBuffer(UniformBuffer(sizeof(UniformCamera), 0)),
		m_objectUniformBuffer(UniformBuffer(sizeof(UniformObject), 1)),

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
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.attributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

		m_cameraUniformBuffer.Create(logicalDevice, physicalDevice, surface);
		m_objectUniformBuffer.Create(logicalDevice, physicalDevice, surface);

		m_shader.Create(logicalDevice);
		m_pipeline.Create(logicalDevice, renderPass, vertexInputState, { m_cameraUniformBuffer.GetDescriptorSetLayout(), m_objectUniformBuffer.GetDescriptorSetLayout() });
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

		m_objectUniformBuffer.Cleanup(logicalDevice);
		m_cameraUniformBuffer.Cleanup(logicalDevice);
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipeline());

		{
			UniformCamera *data = nullptr;
			vkMapMemory(logicalDevice, m_objectUniformBuffer.GetBufferMemory(), 0, m_objectUniformBuffer.GetSize(), 0, reinterpret_cast<void**>(&data));
			data->projection = *camera.GetProjectionMatrix();
			data->view = *camera.GetViewMatrix();
			vkUnmapMemory(logicalDevice, m_objectUniformBuffer.GetBufferMemory());

		//	VkDescriptorSet descriptors[] = { m_cameraUniformBuffer.GetDescriptorSet() };
		//	vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		}
		
		{
			UniformObject *data = nullptr;
			vkMapMemory(logicalDevice, m_objectUniformBuffer.GetBufferMemory(), 0, m_objectUniformBuffer.GetSize(), 0, reinterpret_cast<void**>(&data));
			data->memes = std::sin(Engine::Get()->GetTime()) > 0.0f;
			data->colour = Colour(1.0f, fabs(std::sin(Engine::Get()->GetTime())), 0.3f, 0.4f);
			data->model = Matrix4();
			Matrix4::TransformationMatrix(Vector3(0.2f, 0.1f, 0.1f), Vector3(0.0f, 10.0f, 0.0f), 0.9f, &data->model);
			vkUnmapMemory(logicalDevice, m_objectUniformBuffer.GetBufferMemory());

		//	VkDescriptorSet descriptors[] = { m_objectUniformBuffer.GetDescriptorSet() };
		//	vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipelineLayout(), 1, 1, descriptors, 0, nullptr);
		}


	//	VkDescriptorSet descriptors[] = { m_cameraUniformBuffer.GetDescriptorSet(), m_objectUniformBuffer.GetDescriptorSet() };
	//	vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipelineLayout(), 0, 2, descriptors, 0, nullptr);

		VkBuffer vertexBuffers[] = { m_vertexBuffer.GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(*commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(*commandBuffer, m_indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*commandBuffer, m_indexBuffer.GetIndicesSize(), 1, 0, 0, 0);
	}
}
