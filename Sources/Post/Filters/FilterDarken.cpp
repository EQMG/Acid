#include "FilterDarken.hpp"

namespace acid
{
	FilterDarken::FilterDarken(const GraphicsStage &graphicsStage, const float &factor) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Darken.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_factor(factor)
	{
	}

	void FilterDarken::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("factor", m_factor);

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
