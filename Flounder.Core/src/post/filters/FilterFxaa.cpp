#include "FilterFxaa.hpp"

#include "../../renderer/Renderer.hpp"
#include "../../textures/Texture.hpp"

namespace Flounder
{
	const DescriptorType FilterFxaa::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType FilterFxaa::typeSamplerColour = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);

	FilterFxaa::FilterFxaa(const int &subpass) :
		IPostFilter("filterFxaa", "res/shaders/filters/fxaa.frag.spv", subpass, { typeUboScene, typeSamplerColour }),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_spanMax(8.0f)
	{
	}

	FilterFxaa::~FilterFxaa()
	{
		delete m_uniformScene;
	}

	void FilterFxaa::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.spanMax = m_spanMax;
		m_uniformScene->Update(&uboScene);

		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
		{
			m_uniformScene->GetWriteDescriptor(0, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet)
		};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
