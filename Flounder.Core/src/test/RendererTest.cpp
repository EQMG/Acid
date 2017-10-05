#include "RendererTest.hpp"

#include "../devices/Display.hpp"
#include "../renderer/Renderer.hpp"
#include "TestShader.hpp"

namespace Flounder
{
	RendererTest::RendererTest() :
		IRenderer(),
		m_uniformScene(new UniformBuffer(sizeof(TestShader::UboScene))),

		m_testEntity1(new TestEntity(Vector3(0.0f, -2.3f, 3.0f), Vector3())),
		m_testEntity2(new TestEntity(Vector3(2.0f, -2.3f, 2.0f), Vector3())),

		m_shader(new Shader("tests", {
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		})),
		m_pipeline(new Pipeline("tests", PipelinePolygon, m_shader, TestShader::inputState, TestShader::descriptor))
	{
		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());

		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		descriptorWrites1 = std::vector<VkWriteDescriptorSet>{ m_uniformScene->GetWriteDescriptor(0, descriptorSet), m_testEntity1->m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_testEntity1->m_diffuse->GetWriteDescriptor(2, descriptorSet), m_testEntity1->m_swapMap->GetWriteDescriptor(3, descriptorSet) }; // TODO: Modulaize this!
		descriptorWrites2 = std::vector<VkWriteDescriptorSet>{ m_uniformScene->GetWriteDescriptor(0, descriptorSet), m_testEntity2->m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_testEntity2->m_diffuse->GetWriteDescriptor(2, descriptorSet), m_testEntity2->m_swapMap->GetWriteDescriptor(3, descriptorSet) }; // TODO: Modulaize this!
	}

	RendererTest::~RendererTest()
	{
		delete m_uniformScene;
		delete m_testEntity2;
		delete m_testEntity1;

		delete m_shader;
		delete m_pipeline;
	}

	void RendererTest::Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		TestShader::UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene->Update(&uboScene);

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites1.size()), descriptorWrites1.data(), 0, nullptr);
		m_testEntity1->CmdRender(*commandBuffer, *m_pipeline);

	//	vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites2.size()), descriptorWrites2.data(), 0, nullptr);
	//	m_testEntity2->CmdRender(*commandBuffer, m_pipeline);
	}
}
