#include "DefaultFilter.hpp"

namespace acid {
DefaultFilter::DefaultFilter(const Pipeline::Stage &pipelineStage, bool lastFilter) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Default.frag"}),
	lastFilter(lastFilter) {
}

void DefaultFilter::Render(const CommandBuffer &commandBuffer) {
	// Updates descriptors.
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!descriptorSet.Update(pipeline))
		return;

	// Draws the object.
	pipeline.BindPipeline(commandBuffer);

	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

	// Resets switching for next pass.
	if (lastFilter) {
		GlobalSwitching = 0;
	}
}
}
