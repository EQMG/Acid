#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../renderer/Renderer.hpp"

namespace Flounder
{
	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformBuffer(UniformBuffer(sizeof(UBO), 0, VK_SHADER_STAGE_VERTEX_BIT)),
		m_model("res/treeBirchSmall/model.obj"),
		m_texture("res/treeBirchSmall/diffuse.png"),

		m_shader(Shader("tests", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		})),
		m_pipeline(Pipeline("tests", PipelinePolygon, &m_shader, { &m_uniformBuffer }))
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

		m_uniformBuffer.Create(logicalDevice, physicalDevice, surface);
		m_model.Create();

		m_shader.Create(logicalDevice);
		m_pipeline.Create(logicalDevice, renderPass, vertexInputState);
	}

	RendererTest::~RendererTest()
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		m_shader.Cleanup(logicalDevice);
		m_pipeline.Cleanup(logicalDevice);

		m_model.Cleanup();
		m_uniformBuffer.Cleanup(logicalDevice);
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		const auto logicalDevice = Display::Get()->GetDevice();

		UBO ubo = {};
		ubo.projection = *camera.GetProjectionMatrix();
		ubo.view = *camera.GetViewMatrix();
		ubo.model = Matrix4();
		Matrix4::TransformationMatrix(Vector3(0.0f, -2.3f, 3.0f), Vector3(), 1.0f, &ubo.model);
		m_uniformBuffer.Update(logicalDevice, &ubo);

		VkBuffer vertexBuffers[] = { m_model.GetVertexBuffer().GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		VkDescriptorSet descriptors[] = { m_pipeline.GetDescriptorSet() };
		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipeline());
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		vkCmdBindVertexBuffers(*commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(*commandBuffer, m_model.GetIndexBuffer().GetBuffer(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*commandBuffer, m_model.GetIndexBuffer().GetIndicesSize(), 1, 0, 0, 0);
	}
}
