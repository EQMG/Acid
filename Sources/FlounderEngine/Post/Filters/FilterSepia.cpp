#include "FilterSepia.hpp"

namespace Flounder
{
	FilterSepia::FilterSepia(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Sepia.frag", graphicsStage, {})
	{
	}

	FilterSepia::~FilterSepia()
	{
	}

	void FilterSepia::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates descriptors.
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
