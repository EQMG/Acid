#include "FilterDarken.hpp"

namespace Flounder
{
	FilterDarken::FilterDarken(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Darken.frag", graphicsStage, {}),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_factor(0.5f)
	{
	}

	FilterDarken::~FilterDarken()
	{
		delete m_uniformScene;
	}

	void FilterDarken::Render(const VkCommandBuffer &commandBuffer)
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
		uboScene.factor = m_factor;
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
