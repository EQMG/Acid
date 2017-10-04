#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../renderer/Renderer.hpp"

namespace Flounder
{
	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformBuffer(UniformBuffer(sizeof(UBO), VK_SHADER_STAGE_VERTEX_BIT)),
		m_model(Model("res/treeBirchSmall/model.obj")),
		m_texture(Texture("res/treeBirchSmall/diffuse.png")),

		m_shader(Shader("tests", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		})),
		m_pipeline(Pipeline("tests", PipelinePolygon, &m_shader, { &m_uniformBuffer, &m_texture }))
	{
		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescriptions = Vertex::GetAttributeDescriptions();
		VertexInputState vertexInputState = {};
		vertexInputState.bindingDescriptionCount = 1;
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.attributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

		m_uniformBuffer.Create();
		m_model.Create();
		m_texture.Create();

		m_shader.Create();
		m_pipeline.Create(vertexInputState);
	}

	RendererTest::~RendererTest()
	{
		m_shader.Cleanup();
		m_pipeline.Cleanup();

		m_texture.Cleanup();
		m_model.Cleanup();
		m_uniformBuffer.Cleanup();
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UBO ubo = {};
		ubo.projection = *camera.GetProjectionMatrix();
		ubo.view = *camera.GetViewMatrix();
		ubo.model = Matrix4();
		Matrix4::TransformationMatrix(Vector3(0.0f, -2.3f, 3.0f), Vector3(), 1.0f, &ubo.model);
		m_uniformBuffer.Update(&ubo);

		VkBuffer vertexBuffers[] = { m_model.GetVertexBuffer().GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		VkDescriptorSet descriptors[] = { m_pipeline.GetDescriptorSet() };
		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipeline());
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		vkCmdBindVertexBuffers(*commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(*commandBuffer, m_model.GetIndexBuffer().GetBuffer(), 0, m_model.GetIndexBuffer().GetIndexType());
		vkCmdDrawIndexed(*commandBuffer, m_model.GetIndexBuffer().GetIndexCount(), 1, 0, 0, 0);
	}
}
