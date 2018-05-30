#include "FilterBlurVertical.hpp"

namespace fl
{
	FilterBlurVertical::FilterBlurVertical(const GraphicsStage &graphicsStage, const float &scale) :
		IPostFilter({"Resources/Shaders/Filters/Default.vert", "Resources/Shaders/Filters/BlurVertical.frag"}, graphicsStage, {}),
		m_uniformScene(UniformHandler()),
		m_scale(scale),
		m_height(0.0f)
	{
	}

	FilterBlurVertical::~FilterBlurVertical()
	{
	}

	void FilterBlurVertical::Render(const CommandBuffer &commandBuffer)
	{
		m_height = Display::Get()->GetHeight();

		// Updates uniforms.
		m_uniformScene.Push("height", m_height);
		m_uniformScene.Push("scale", m_scale);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(2));
		bool descriptorsSet = m_descriptorSet.Update(m_pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Binds the pipeline.
		m_pipeline.BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet.GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
