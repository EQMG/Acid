#include "FilterTiltshift.hpp"

namespace acid
{
	FilterTiltshift::FilterTiltshift(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Tiltshift.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_blurAmount(1.0f),
		m_centre(1.1f),
		m_stepSize(0.004f),
		m_steps(3.0f)
	{
	}

	void FilterTiltshift::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("blurAmount", m_blurAmount);
		m_uniformScene.Push("centre", m_centre);
		m_uniformScene.Push("stepSize", m_stepSize);
		m_uniformScene.Push("steps", m_steps);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(5));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(5));
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
