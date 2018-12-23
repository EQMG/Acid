#include "FilterLensflare.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
	FilterLensflare::FilterLensflare(const GraphicsStage &graphicsStage) :
		PostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Lensflare.frag"}, {}),
		m_pushScene(PushHandler()),
		m_sunPosition(Vector3()),
		m_sunHeight(0.0f)
	{
	}

	void FilterLensflare::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
		// Updates uniforms.
		m_pushScene.Push("sunPosition", m_sunPosition);
		m_pushScene.Push("worldHeight", m_sunHeight);
		m_pushScene.Push("displaySize", Vector2(static_cast<float>(m_pipeline.GetWidth()), static_cast<float>(m_pipeline.GetHeight())));

		// Updates descriptors.
		m_descriptorSet.Push("PushScene", m_pushScene);
	//	m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//	m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
		m_descriptorSet.Push("samplerMaterial", GetAttachment("samplerMaterial", "materials"));
		PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pushScene.BindPush(commandBuffer, m_pipeline);
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
