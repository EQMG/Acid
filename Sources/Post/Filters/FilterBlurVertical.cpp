#include "FilterBlurVertical.hpp"

#include "Display/Display.hpp"

namespace acid
{
	FilterBlurVertical::FilterBlurVertical(const GraphicsStage &graphicsStage, const float &scale) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/BlurVertical.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_scale(scale),
		m_height(0.0f)
	{
	}

	FilterBlurVertical::~FilterBlurVertical()
	{
	}

	void FilterBlurVertical::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		m_height = Display::Get()->GetHeight();

		// Updates uniforms.
		m_uniformScene.Push("height", m_height);
		m_uniformScene.Push("scale", m_scale);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeAlbedo", m_pipeline.GetTexture(3));
		m_descriptorSet.Push("samplerAlbedo", m_pipeline.GetTexture(3));
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
