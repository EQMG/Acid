#include "FilterPixel.hpp"

namespace Flounder
{
	FilterPixel::FilterPixel(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Pixel.frag", graphicsStage, {}),
		m_uniformScene(new UniformHandler()),
		m_pixelSize(2.0f)
	{
	}

	FilterPixel::~FilterPixel()
	{
		delete m_uniformScene;
	}

	void FilterPixel::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_uniformScene->Push("pixelSize", m_pixelSize);

		// Updates descriptors.
		m_descriptorSet->Push("UboScene", m_uniformScene);
		m_descriptorSet->Push("writeColour", m_pipeline->GetTexture(2));
		m_descriptorSet->Push("samplerColour", m_pipeline->GetTexture(2));
		bool descriptorsSet = m_descriptorSet->Update(*m_pipeline);

		if (!descriptorsSet)
		{
			return;
		}

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->GetDescriptorSet()->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
