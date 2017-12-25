#include "FilterBlurHorizontal.hpp"

#include "../../Devices/Display.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
	{
		UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
		Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
	};

	FilterBlurHorizontal::FilterBlurHorizontal(const int &subpass, const float &sizeScalar) :
		IPostFilter("Resources/Shaders/Filters/BlurHorizontal.frag.spv", subpass, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_size(Vector2()),
		m_blurAmount(2.0f),
		m_fitToDisplay(true),
		m_sizeScalar(sizeScalar)
	{
	}

	FilterBlurHorizontal::FilterBlurHorizontal(const int &subpass, const int &width, const int &height) :
		IPostFilter("Resources/Shaders/Filters/BlurHorizontal.frag.spv", subpass, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_size(Vector2(width, height)),
		m_blurAmount(2.0f),
		m_fitToDisplay(false),
		m_sizeScalar(-1.0f)
	{
	}

	FilterBlurHorizontal::~FilterBlurHorizontal()
	{
		delete m_uniformScene;
	}

	void FilterBlurHorizontal::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		if (m_fitToDisplay)
		{
			Vector2 newSize = Vector2(
				static_cast<int>(Display::Get()->GetWidth() * m_sizeScalar),
				static_cast<int>(Display::Get()->GetHeight() * m_sizeScalar)
			);

			if (m_size != newSize)
			{
				m_size = newSize;
				// TODO: Recreate image.
			}
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
