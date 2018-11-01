#include "FilterWobble.hpp"

namespace acid
{
	FilterWobble::FilterWobble(const GraphicsStage &graphicsStage, const float &wobbleSpeed) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Wobble.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_wobbleSpeed(wobbleSpeed),
		m_wobbleAmount(0.0f)
	{
	}

	void FilterWobble::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->GetDeltaRender().AsSeconds();

		// Updates uniforms.
		m_uniformScene.Push("moveIt", m_wobbleAmount);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", Renderer::Get()->GetAttachment("resolved"));
		m_descriptorSet.Push("samplerColour", Renderer::Get()->GetAttachment("resolved"));
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
}
