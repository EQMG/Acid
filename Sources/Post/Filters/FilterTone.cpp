#include "FilterTone.hpp"

namespace fl
{
	FilterTone::FilterTone(const GraphicsStage &graphicsStage) :
		IPostFilter({"Shaders/Filters/Default.vert", "Shaders/Filters/Tone.frag"}, graphicsStage, {})
	{
	}

	FilterTone::~FilterTone()
	{
	}

	void FilterTone::Render(const CommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		m_descriptorSet.Push("writeColour", m_pipeline.GetTexture(2));
		m_descriptorSet.Push("samplerColour", m_pipeline.GetTexture(2));
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
