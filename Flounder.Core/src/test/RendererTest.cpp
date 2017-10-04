#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../renderer/Renderer.hpp"
#include "TestUbos.hpp"

namespace Flounder
{
	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformScene(UniformBuffer(sizeof(UboScene), VK_SHADER_STAGE_VERTEX_BIT)),

		m_testEntity(TestEntity()),

		m_shader(Shader("tests", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		})),
		m_pipeline(Pipeline("tests", PipelinePolygon, &m_shader, { &m_uniformScene, &m_testEntity.m_uniformObject, &m_testEntity.m_diffuse, &m_testEntity.m_swapMap }))
	{
		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescriptions = Vertex::GetAttributeDescriptions();
		VertexInputState vertexInputState = {};
		vertexInputState.bindingDescriptionCount = 1;
		vertexInputState.pVertexBindingDescriptions = &bindingDescription;
		vertexInputState.attributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

		m_uniformScene.Create();
		m_testEntity.Create();

		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());

		m_shader.Create();
		m_pipeline.Create(vertexInputState);
	}

	RendererTest::~RendererTest()
	{
		m_shader.Cleanup();
		m_pipeline.Cleanup();

		m_testEntity.Cleanup();
		m_uniformScene.Cleanup();
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene.Update(&uboScene);

		m_testEntity.CmdRender(*commandBuffer, m_pipeline);
	}
}
