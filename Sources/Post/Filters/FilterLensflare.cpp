#include "FilterLensflare.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	FilterLensflare::FilterLensflare(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Lensflare.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_sunPosition(Vector3()),
		m_sunHeight(0.0f)
	{
	}

	FilterLensflare::~FilterLensflare()
	{
	}

	void FilterLensflare::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("sunPosition", m_sunPosition);
		m_uniformScene.Push("worldHeight", m_sunHeight);
		m_uniformScene.Push("displaySize", Vector2(static_cast<float>(Display::Get()->GetWidth()), static_cast<float>(Display::Get()->GetHeight())));

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerMaterial", m_pipeline.GetTexture(4));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}

	void FilterLensflare::SetSunPosition(const Vector3 &sunPosition)
	{
		auto camera = Scenes::Get()->GetCamera();
		m_sunPosition = Matrix4::WorldToScreenSpace(sunPosition, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
}
