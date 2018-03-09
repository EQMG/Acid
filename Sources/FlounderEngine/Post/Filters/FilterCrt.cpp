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

	void FilterCrt::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->Update({
			m_uniformScene,
			m_pipeline->GetTexture(2),
			m_pipeline->GetTexture(2)
		});

		// Updates uniforms.
		UboScene uboScene = {};
		uboScene.screenColour = *m_screenColour;
		uboScene.curveAmountX = m_curveAmountX * Display::Get()->GetAspectRatio();
		uboScene.curveAmountY = m_curveAmountY;
		uboScene.scanLineSize = m_scanLineSize;
		uboScene.scanIntensity = m_scanIntensity;
		uboScene.moveTime = Engine::Get()->GetTime() / 100.0f;
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
