#include "FilterLensflare.hpp"

#include "../../Scenes/Scenes.hpp"
#include "../../Devices/Display.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerColour
			Texture::CreateDescriptor(3, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerMaterial
		};

	FilterLensflare::FilterLensflare(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Lensflare.frag", graphicsStage, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_sunPosition(new Vector3()),
		m_sunHeight(0.0f)
	{
	}

	FilterLensflare::~FilterLensflare()
	{
		delete m_uniformScene;
		delete m_sunPosition;
	}

	void FilterLensflare::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->Update({
			m_uniformScene,
			m_pipeline->GetTexture(2),
			m_pipeline->GetTexture(2),
			m_pipeline->GetTexture(4)
		});

		// Updates uniforms.
		UboScene uboScene = {};
		uboScene.sunPosition = *m_sunPosition;
		uboScene.worldHeight = m_sunHeight;
		uboScene.displaySize = Vector2(static_cast<float>(Display::Get()->GetWidth()), static_cast<float>(Display::Get()->GetHeight()));
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}

	void FilterLensflare::SetSunPosition(const Vector3 &sunPosition) const
	{
		ICamera *camera = Scenes::Get()->GetCamera();
		Matrix4::WorldToScreenSpace(sunPosition, *camera->GetViewMatrix(), *camera->GetProjectionMatrix(), m_sunPosition);
	}
}
