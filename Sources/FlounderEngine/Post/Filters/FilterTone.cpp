#include "FilterTone.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	FilterTone::FilterTone(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Tone.frag", graphicsStage, { })
	{
	}

	FilterTone::~FilterTone()
	{
	}

	void FilterTone::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->Update({
			m_pipeline->GetTexture(2),
			m_pipeline->GetTexture(2)
		});

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
