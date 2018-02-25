#include "FilterCrt.hpp"

#include "../../Devices/Display.hpp"
#include "../../Textures/Texture.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
		};

	FilterCrt::FilterCrt(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Crt.frag.spv", graphicsStage, DESCRIPTORS),
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

	void FilterCrt::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.screenColour = *m_screenColour;
		uboScene.curveAmountX = m_curveAmountX * Display::Get()->GetAspectRatio();
		uboScene.curveAmountY = m_curveAmountY;
		uboScene.scanLineSize = m_scanLineSize;
		uboScene.scanIntensity = m_scanIntensity;
		uboScene.moveTime = Engine::Get()->GetTime() / 100.0f;
		m_uniformScene->Update(&uboScene);

		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
			{
				m_uniformScene->GetWriteDescriptor(0, descriptorSet),
				m_pipeline->GetTexture(2)->GetWriteDescriptor(1, descriptorSet),
				m_pipeline->GetTexture(2)->GetWriteDescriptor(2, descriptorSet)
			};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
