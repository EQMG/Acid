#include "FilterNegative.hpp"

namespace acid
{
	FilterNegative::FilterNegative(const GraphicsStage &graphicsStage) :
		PostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Negative.frag"}, {})
	{
	}

	void FilterNegative::Render(const CommandBuffer &commandBuffer)
	{
		// Updates descriptors.
	//	m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//	m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
		PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		m_model->CmdRender(commandBuffer);
	}
}
