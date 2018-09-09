#include "FilterPixel.hpp"

namespace acid
{
	FilterPixel::FilterPixel(const GraphicsStage &graphicsStage) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Pixel.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_pixelSize(2.0f)
	{
	}

	void FilterPixel::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("pixelSize", m_pixelSize);

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
