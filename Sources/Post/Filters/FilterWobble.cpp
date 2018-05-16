#include "FilterWobble.hpp"

namespace fl
{
	FilterWobble::FilterWobble(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Wobble.frag", graphicsStage, {}),
		m_uniformScene(new UniformHandler()),
		m_wobbleSpeed(2.0f),
		m_wobbleAmount(0.0f)
	{
	}

	FilterWobble::~FilterWobble()
	{
		delete m_uniformScene;
	}

	void FilterWobble::Render(const CommandBuffer &commandBuffer)
	{
		m_wobbleAmount += m_wobbleSpeed * Engine::Get()->GetDeltaRender();

		// Updates uniforms.
		m_uniformScene->Push("moveIt", m_wobbleAmount);

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
