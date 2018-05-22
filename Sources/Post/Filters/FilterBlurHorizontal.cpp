#include "FilterBlurHorizontal.hpp"

namespace fl
{
	FilterBlurHorizontal::FilterBlurHorizontal(const GraphicsStage &graphicsStage, const float &scale) :
		IPostFilter({"Resources/Shaders/Filters/Default.vert", "Resources/Shaders/Filters/BlurHorizontal.frag"}, graphicsStage, {}),
		m_uniformScene(new UniformHandler()),
		m_scale(scale),
		m_width(0.0f)
	{
	}

	FilterBlurHorizontal::~FilterBlurHorizontal()
	{
		delete m_uniformScene;
	}

	void FilterBlurHorizontal::Render(const CommandBuffer &commandBuffer)
	{
		m_width = Display::Get()->GetHeight();

		// Updates uniforms.
		m_uniformScene->Push("width", m_width);
		m_uniformScene->Push("scale", m_scale);

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", m_uniformScene);
		m_descriptorSet->Push("writeColour", m_pipeline->GetTexture(2));
		m_descriptorSet->Push("samplerColour", m_pipeline->GetTexture(2));
		bool descriptorsSet = m_descriptorSet->Update(*m_pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Binds the pipeline.
		m_pipeline->BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
