#include "FilterCrt.hpp"

#include "../../devices/Display.hpp"
#include "../../textures/Texture.hpp"
#include "../../renderer/Renderer.hpp"

namespace Flounder
{
	const DescriptorType FilterCrt::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType FilterCrt::typeSamplerColour = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);

	FilterCrt::FilterCrt(const int &subpass) :
		IPostFilter("filterCrt", "res/shaders/filters/crt.frag.spv", subpass, { typeUboScene, typeSamplerColour }),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_screenColour(new Colour(0.5f, 1.0f, 0.5f)),
		m_curveAmountX(0.1f),
		m_curveAmountY(0.1f),
		m_scanLineSize(1000.0f),
		m_scanIntensity(0.1f)
	{
	}

	FilterCrt::~FilterCrt()
	{
		delete m_uniformScene;
		delete m_screenColour;
	}

	void FilterCrt::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.screenColour = *m_screenColour;
		uboScene.curveAmountX = m_curveAmountX * static_cast<float>(Display::Get()->GetAspectRatio());
		uboScene.curveAmountY = m_curveAmountY;
		uboScene.scanLineSize = m_scanLineSize;
		uboScene.scanIntensity = m_scanIntensity;
		uboScene.moveTime = static_cast<float>(Engine::Get()->GetTime()) / 100.0f;
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
