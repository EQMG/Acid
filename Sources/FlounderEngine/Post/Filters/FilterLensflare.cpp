#include "FilterLensflare.hpp"

#include "Scenes/Scenes.hpp"

namespace Flounder
{
	FilterLensflare::FilterLensflare(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Lensflare.frag", graphicsStage, {}),
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

		m_descriptorSet->UpdateMap({
			{"UboScene",        m_uniformScene},
			{"writeColour",     m_pipeline->GetTexture(2)},
			{"samplerColour",   m_pipeline->GetTexture(2)},
			{"samplerMaterial", m_pipeline->GetTexture(4)}
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
