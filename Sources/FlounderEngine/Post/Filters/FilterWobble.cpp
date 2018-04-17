#include "FilterWobble.hpp"

namespace Flounder
{
	FilterWobble::FilterWobble(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Wobble.frag", graphicsStage, {}),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_wobbleSpeed(2.0f),
		m_wobbleAmount(0.0f)
	{
	}

	FilterWobble::~FilterWobble()
	{
		delete m_uniformScene;
	}

	void FilterWobble::Render(const VkCommandBuffer &commandBuffer)
	{
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->GetDeltaRender();

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
		uboScene.moveIt = m_wobbleAmount;
		m_uniformScene->Update(&uboScene);

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
