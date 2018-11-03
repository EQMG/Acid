#include "FilterTiltshift.hpp"

namespace acid
{
	FilterTiltshift::FilterTiltshift(const GraphicsStage &graphicsStage, const float &blurAmount, const float &centre, const float &stepSize, const float &steps) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Tiltshift.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_blurAmount(blurAmount),
		m_centre(centre),
		m_stepSize(stepSize),
		m_steps(steps)
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
		m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
		m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
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
