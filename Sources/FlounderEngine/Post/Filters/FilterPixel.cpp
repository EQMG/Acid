#include "FilterPixel.hpp"

#include "../../Devices/Display.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
		UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
		Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
	};

	FilterPixel::FilterPixel(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Pixel.frag.spv", subpass, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_pixelSize(2.0f)
	{
	}

	FilterPixel::~FilterPixel()
	{
		delete m_uniformScene;
	}

	void FilterPixel::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.pixelSize = m_pixelSize;
		uboScene.displaySize = Vector2(Display::Get()->GetWidth(), Display::Get()->GetHeight());
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
