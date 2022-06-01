#include "GrainFilter.hpp"

namespace acid {
GrainFilter::GrainFilter(const Pipeline::Stage &pipelineStage, float strength) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Grain.frag"}),
	strength(strength) {
}

void GrainFilter::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	pushScene.Push("strength", strength);

	// Updates descriptors.
	descriptorSet.Push("PushScene", pushScene);
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!descriptorSet.Update(pipeline))
		return;

	// Binds the pipeline.
	pipeline.BindPipeline(commandBuffer);

	// Draws the object.
	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	pushScene.BindPush(commandBuffer, pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
