#include "FilterBlit.hpp"

namespace acid {
FilterBlit::FilterBlit(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Blit.frag"}) {
}

void FilterBlit::Render(const CommandBuffer &commandBuffer) {
	// Updates descriptors.
	descriptorSet.Push("samplerColour", Graphics::Get()->GetAttachment("swapchain"));

	if (!descriptorSet.Update(pipeline))
		return;

	// Draws the object.
	pipeline.BindPipeline(commandBuffer);

	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
