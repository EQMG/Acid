#include "FilterBlurVertical.hpp"

#include "../../Devices/Display.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
		UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
		Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
	};

	FilterBlurVertical::FilterBlurVertical(const int &subpass, const float &sizeScalar) :
		IPostFilter("Resources/Shaders/Filters/BlurVertical.frag.spv", subpass, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_size(Vector2(static_cast<int>(Display::Get()->GetWidth() * sizeScalar), static_cast<int>(Display::Get()->GetWidth() * sizeScalar))),
		m_blurAmount(2.0f),
		m_fitToDisplay(true),
		m_sizeScalar(sizeScalar)
	{
	}

	FilterBlurVertical::FilterBlurVertical(const int &subpass, const int &width, const int &height) :
		IPostFilter("Resources/Shaders/Filters/BlurVertical.frag.spv", subpass, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_size(Vector2(width, height)),
		m_blurAmount(2.0f),
		m_fitToDisplay(false),
		m_sizeScalar(-1.0f)
	{
	}

	FilterBlurVertical::~FilterBlurVertical()
	{
		delete m_uniformScene;
	}

	void FilterBlurVertical::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		if (m_fitToDisplay)
		{
			m_size.m_x = static_cast<int>(Display::Get()->GetWidth() * m_sizeScalar);
			m_size.m_y = static_cast<int>(Display::Get()->GetHeight() * m_sizeScalar);
		}

		UboScene uboScene = {};
		uboScene.blurAmount = m_blurAmount;
		uboScene.size = m_size;
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
