#include "FilterVignette.hpp"

namespace acid
{
	FilterVignette::FilterVignette(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Vignette.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_innerRadius(0.15f),
		m_outerRadius(1.35f),
		m_opacity(0.85f)
	{
	}

	void FilterVignette::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("innerRadius", m_innerRadius);
		m_uniformScene.Push("outerRadius", m_outerRadius);
		m_uniformScene.Push("opacity", m_opacity);

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
