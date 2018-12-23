#include "FilterWobble.hpp"

namespace acid
{
	FilterWobble::FilterWobble(const GraphicsStage &graphicsStage, const float &wobbleSpeed) :
		PostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Wobble.frag"}, {}),
		m_pushScene(PushHandler()),
		m_wobbleSpeed(wobbleSpeed),
		m_wobbleAmount(0.0f)
	{
	}

	void FilterWobble::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->GetDeltaRender().AsSeconds();

		// Updates uniforms.
		m_pushScene.Push("moveIt", m_wobbleAmount);

		// Updates descriptors.
		m_descriptorSet.Push("PushScene", m_pushScene);
	//	m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//	m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
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
}
