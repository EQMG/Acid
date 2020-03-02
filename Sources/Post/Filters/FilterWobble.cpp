#include "FilterWobble.hpp"

namespace acid {
FilterWobble::FilterWobble(const Pipeline::Stage &pipelineStage, float wobbleSpeed) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Wobble.frag"}),
	wobbleSpeed(wobbleSpeed),
	wobbleAmount(0.0f) {
}

void FilterWobble::Render(const CommandBuffer &commandBuffer) {
	wobbleAmount += wobbleSpeed * Engine::Get()->GetDeltaRender().AsSeconds();

	// Updates uniforms.
	pushScene.Push("moveIt", wobbleAmount);

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
