#include "FilterDarken.hpp"

namespace acid
{
	FilterDarken::FilterDarken(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Darken.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_factor(0.5f)
	{
	}

	FilterDarken::~FilterDarken()
	{
	}

	void FilterDarken::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("factor", m_factor);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(6));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(6));
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
