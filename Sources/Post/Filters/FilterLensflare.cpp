#include "FilterLensflare.hpp"

#include "Scenes/Scenes.hpp"

namespace fl
{
	FilterLensflare::FilterLensflare(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Lensflare.frag", graphicsStage, {}),
		m_uniformScene(new UniformHandler()),
		m_sunPosition(new Vector3()),
		m_sunHeight(0.0f)
	{
	}

	FilterLensflare::~FilterLensflare()
	{
		delete m_uniformScene;
		delete m_sunPosition;
	}

	void FilterLensflare::Render(const CommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_uniformScene->Push("sunPosition", *m_sunPosition);
		m_uniformScene->Push("worldHeight", m_sunHeight);
		m_uniformScene->Push("displaySize", Vector2(static_cast<float>(Display::Get()->GetWidth()), static_cast<float>(Display::Get()->GetHeight())));

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", m_uniformScene);
		m_descriptorSet->Push("writeColour", m_pipeline->GetTexture(2));
		m_descriptorSet->Push("samplerColour", m_pipeline->GetTexture(2));
		m_descriptorSet->Push("samplerMaterial", m_pipeline->GetTexture(4));
		bool descriptorsSet = m_descriptorSet->Update(*m_pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}

	void FilterLensflare::SetSunPosition(const Vector3 &sunPosition) const
	{
		ICamera *camera = Scenes::Get()->GetCamera();
		*m_sunPosition = Matrix4::WorldToScreenSpace(sunPosition, *camera->GetViewMatrix(), *camera->GetProjectionMatrix());
	}
}
