#include "CrtFilter.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
CrtFilter::CrtFilter(const Pipeline::Stage &pipelineStage, const Colour &screenColour, float curveAmountX, float curveAmountY, float scanLineSize,
	float scanIntensity) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Crt.frag"}),
	screenColour(screenColour),
	curveAmountX(curveAmountX),
	curveAmountY(curveAmountY),
	scanLineSize(scanLineSize),
	scanIntensity(scanIntensity) {
}

void CrtFilter::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	pushScene.Push("screenColour", screenColour);
	pushScene.Push("curveAmountX", curveAmountX * pipeline.GetRenderArea().GetAspectRatio());
	pushScene.Push("curveAmountY", curveAmountY);
	pushScene.Push("scanLineSize", scanLineSize);
	pushScene.Push("scanIntensity", scanIntensity);
	pushScene.Push("moveTime", Time::Now() / 100.0f);

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
