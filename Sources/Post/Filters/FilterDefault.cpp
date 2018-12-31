#include "FilterDefault.hpp"

namespace acid
{
	FilterDefault::FilterDefault(const GraphicsStage &graphicsStage, const bool &lastFilter) :
		PostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Default.frag"}, {}),
		m_lastFilter(lastFilter)
	{
	}

	void FilterDefault::Render(const CommandBuffer &commandBuffer, const Camera &camera)
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

		// Resets switching for next pass.
		if (m_lastFilter)
		{
			GLOBAL_SWITCHING = 0;
		}
	}
}
