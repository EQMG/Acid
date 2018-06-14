#include "FilterPixel.hpp"

namespace fl
{
	FilterPixel::FilterPixel(const GraphicsStage &graphicsStage) :
		IPostFilter({"Shaders/Filters/Default.vert", "Shaders/Filters/Pixel.frag"}, graphicsStage, {}),
		m_uniformScene(UniformHandler()),
		m_pixelSize(2.0f)
	{
	}

	FilterPixel::~FilterPixel()
	{
	}

	void FilterPixel::Render(const CommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_uniformScene.Push("pixelSize", m_pixelSize);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(2));
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
