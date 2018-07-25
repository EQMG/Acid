#include "FilterBlurHorizontal.hpp"

namespace acid
{
	FilterBlurHorizontal::FilterBlurHorizontal(const GraphicsStage &graphicsStage, const float &scale) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/BlurHorizontal.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_scale(scale),
		m_width(0.0f)
	{
	}

	FilterBlurHorizontal::~FilterBlurHorizontal()
	{
	}

	void FilterBlurHorizontal::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_width = Display::Get()->GetHeight();

		// Updates uniforms.
		m_uniformScene.Push("width", m_width);
		m_uniformScene.Push("scale", m_scale);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(2));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Binds the pipeline.
		m_pipeline.BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
