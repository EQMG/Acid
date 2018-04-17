#include "FilterPixel.hpp"

namespace Flounder
{
	FilterPixel::FilterPixel(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Pixel.frag", graphicsStage, {}),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_pixelSize(2.0f)
	{
	}

	FilterPixel::~FilterPixel()
	{
		delete m_uniformScene;
	}

	void FilterPixel::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->UpdateMap({
			{"UboScene",      m_uniformScene},
			{"writeColour",   m_pipeline->GetTexture(2)},
			{"samplerColour", m_pipeline->GetTexture(2)}
		});

		// Updates uniforms.
		UboScene uboScene = {};
		uboScene.pixelSize = m_pixelSize;
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
