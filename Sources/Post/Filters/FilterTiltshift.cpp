#include "FilterTiltshift.hpp"

namespace acid {
FilterTiltshift::FilterTiltshift(const Pipeline::Stage &pipelineStage, float blurAmount, float centre, float stepSize, float steps) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Tiltshift.frag"}),
	blurAmount(blurAmount),
	centre(centre),
	stepSize(stepSize),
	steps(steps) {
}

void FilterTiltshift::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	pushScene.Push("blurAmount", blurAmount);
	pushScene.Push("centre", centre);
	pushScene.Push("stepSize", stepSize);
	pushScene.Push("steps", steps);

	// Updates descriptors.
	descriptorSet.Push("PushScene", pushScene);
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!descriptorSet.Update(pipeline))
		return;

	// Draws the object.
	pipeline.BindPipeline(commandBuffer);

	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	pushScene.BindPush(commandBuffer, pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
