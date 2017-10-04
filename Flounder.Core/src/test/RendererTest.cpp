#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../renderer/Renderer.hpp"

namespace Flounder
{
	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformScene(UniformBuffer(sizeof(UboScene), VK_SHADER_STAGE_VERTEX_BIT)),

		m_uniformObject(UniformBuffer(sizeof(UboObject), VK_SHADER_STAGE_VERTEX_BIT)),
		m_model(Model("res/treeBirchSmall/model.obj")),
		m_diffuse(Texture("res/treeBirchSmall/diffuse.png", VK_SHADER_STAGE_FRAGMENT_BIT)),
		m_swapMap(Texture("res/treeBirchSmall/sway.png", VK_SHADER_STAGE_VERTEX_BIT)),

		m_shader(Shader("tests", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		})),
		m_pipeline(Pipeline("tests", PipelinePolygon, &m_shader, { &m_uniformScene, &m_uniformObject, &m_diffuse, &m_swapMap }))
	{
		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescriptions = Vertex::GetAttributeDescriptions();
		VertexInputState vertexInputState = {};
		vertexInputState.bindingDescriptionCount = 1;
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.attributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

		m_uniformScene.Create();
		m_uniformObject.Create();
		m_model.Create();
		m_diffuse.Create();
		m_swapMap.Create();

		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());

		m_shader.Create();
		m_pipeline.Create(vertexInputState);
	}

	RendererTest::~RendererTest()
	{
		m_shader.Cleanup();
		m_pipeline.Cleanup();

		m_swapMap.Cleanup();
		m_diffuse.Cleanup();
		m_model.Cleanup();
		m_uniformObject.Cleanup();
		m_uniformScene.Cleanup();
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene.Update(&uboScene);

		UboObject uboObject = {};
		uboObject.model = Matrix4();
		uboObject.swaying = 1.0f;
		const float systemTime = Engine::Get()->GetTime();
		const float swayX = 0.24f * (sin(0.25f * systemTime) - sin(1.2f * systemTime) + cos(0.5f * systemTime));
		const float swayY = 0.24f * (cos(0.25f * systemTime) - cos(1.2f * systemTime) + sin(0.5f * systemTime));
		uboObject.swayOffset = Vector2(swayX, swayY);
		Matrix4::TransformationMatrix(Vector3(0.0f, -2.3f, 3.0f), Vector3(), 1.0f, &uboObject.model);
		m_uniformObject.Update(&uboObject);

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
