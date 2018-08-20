#include "FilterWobble.hpp"

namespace acid
{
	FilterWobble::FilterWobble(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Wobble.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_wobbleSpeed(2.0f),
		m_wobbleAmount(0.0f)
	{
	}

	FilterWobble::~FilterWobble()
	{
	}

	void FilterWobble::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->GetDeltaRender();

		// Updates uniforms.
		m_uniformScene.Push("moveIt", m_wobbleAmount);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(1));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(1));
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
