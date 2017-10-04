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
		m_pipeline(Pipeline("tests", PipelinePolygon, &m_shader))
	{
		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescriptions = Vertex::GetAttributeDescriptions();
		InputState inputState = {};
		inputState.vertexBindingDescriptionCount = 1;
		inputState.pVertexBindingDescriptions = &bindingDescription;
		inputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		inputState.pVertexAttributeDescriptions = attributeDescriptions.data();
		
		Descriptor descriptor = {};
		descriptor.bindingCount = 4;
		descriptor.pBindings = std::array<VkDescriptorSetLayoutBinding, 4>{ m_uniformScene.GetDescriptorLayout(0), m_testEntity.m_uniformObject.GetDescriptorLayout(1), m_testEntity.m_diffuse.GetDescriptorLayout(2), m_testEntity.m_swapMap.GetDescriptorLayout(3) }.data();
		descriptor.poolSizeCount = 4;
		descriptor.pPoolSizes = std::array<VkDescriptorPoolSize, 4>{ m_uniformScene.GetDescriptorPool(0), m_testEntity.m_uniformObject.GetDescriptorPool(1), m_testEntity.m_diffuse.GetDescriptorPool(2), m_testEntity.m_swapMap.GetDescriptorPool(3) }.data();
	//	descriptor.descriptorWriteCount = 0;// 4;
	//	descriptor.pDescriptorWrites = nullptr; //  std::array<VkWriteDescriptorSet, 4>{ m_uniformScene.GetWriteDescriptor(0), m_testEntity.m_uniformObject.GetWriteDescriptor(1), m_testEntity.m_diffuse.GetWriteDescriptor(2), m_testEntity.m_swapMap.GetWriteDescriptor(3) }.data();

		m_uniformScene.Create();
		m_testEntity.Create();

		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());

		m_shader.Create();
		m_pipeline.Create(inputState, descriptor);
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
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkDescriptorSet descriptorSet = m_pipeline.GetDescriptorSet();
		uint32_t descriptorWriteCount = 4;
		VkWriteDescriptorSet *pDescriptorWrites = std::array<VkWriteDescriptorSet, 4>{ m_uniformScene.GetWriteDescriptor(0, descriptorSet), m_testEntity.m_uniformObject.GetWriteDescriptor(1, descriptorSet), m_testEntity.m_diffuse.GetWriteDescriptor(2, descriptorSet), m_testEntity.m_swapMap.GetWriteDescriptor(3, descriptorSet) }.data();
		vkUpdateDescriptorSets(logicalDevice, descriptorWriteCount, pDescriptorWrites, 0, nullptr);

		UboScene uboScene = {};
		uboScene.projection = *camera.GetProjectionMatrix();
		uboScene.view = *camera.GetViewMatrix();
		m_uniformScene.Update(&uboScene);

		m_testEntity.CmdRender(*commandBuffer, m_pipeline);
	}
}
