#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../renderer/Renderer.hpp"
#include "TestShader.hpp"

namespace Flounder
{
	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformScene(UniformBuffer(sizeof(TestShader::UboScene))),

		m_testEntity1(TestEntity(Vector3(0.0f, -2.3f, 3.0f), Vector3())),
		m_testEntity2(TestEntity(Vector3(2.0f, -2.3f, 2.0f), Vector3())),

		m_shader(Shader("tests", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		})),
		m_pipeline(Pipeline("tests", PipelinePolygon, &m_shader))
	{
		m_uniformScene.Create();
		m_testEntity1.Create();
		m_testEntity2.Create();

		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());

		m_shader.Create();
		m_pipeline.Create(TestShader::inputState, TestShader::descriptor);
	}

	RendererTest::~RendererTest()
	{
		m_shader.Cleanup();
		m_pipeline.Cleanup();

		m_testEntity2.Cleanup();
		m_testEntity1.Cleanup();
		m_uniformScene.Cleanup();
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline.GetDescriptorSet();

		TestShader::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene.Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.GetPipeline());

		{
			std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ m_uniformScene.GetWriteDescriptor(0, descriptorSet), m_testEntity1.m_uniformObject.GetWriteDescriptor(1, descriptorSet), m_testEntity1.m_diffuse.GetWriteDescriptor(2, descriptorSet), m_testEntity1.m_swapMap.GetWriteDescriptor(3, descriptorSet) }; // TODO: Modulaize this!
			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
			m_testEntity1.CmdRender(*commandBuffer, m_pipeline);
		}
		{
			std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ m_uniformScene.GetWriteDescriptor(0, descriptorSet), m_testEntity2.m_uniformObject.GetWriteDescriptor(1, descriptorSet), m_testEntity2.m_diffuse.GetWriteDescriptor(2, descriptorSet), m_testEntity2.m_swapMap.GetWriteDescriptor(3, descriptorSet) }; // TODO: Modulaize this!
			vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
			m_testEntity2.CmdRender(*commandBuffer, m_pipeline);
		}
	}
}
