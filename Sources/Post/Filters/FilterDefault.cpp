#include "FilterDefault.hpp"

namespace acid {
FilterDefault::FilterDefault(const Pipeline::Stage &pipelineStage, bool lastFilter) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Default.frag"}),
	m_lastFilter(lastFilter) {
}

void FilterDefault::Render(const CommandBuffer &commandBuffer) {
	// Updates descriptors.
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!m_descriptorSet.Update(m_pipeline)) {
		return;
	}

	// Draws the object.
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	// Resets switching for next pass.
	if (m_lastFilter) {
		GlobalSwitching = 0;
	}
}
}
