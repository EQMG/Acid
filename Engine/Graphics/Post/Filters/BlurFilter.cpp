#include "BlurFilter.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
BlurFilter::BlurFilter(const Pipeline::Stage &pipelineStage, const Vector2f &direction, Type type) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Blur.frag"}, GetDefines(type)),
	type(type),
	direction(direction) {
}

void BlurFilter::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	pushScene.Push("direction", direction);

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

std::vector<Shader::Define> BlurFilter::GetDefines(const Type &type) {
	std::vector<Shader::Define> defines;
	defines.emplace_back(Shader::Define("BLUR_TYPE", String::To(type)));
	return defines;
}
}
