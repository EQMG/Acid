#include "FilterLensflare.hpp"

#include "Scenes/Scenes.hpp"

namespace acid {
FilterLensflare::FilterLensflare(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Lensflare.frag"}) {
}

void FilterLensflare::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	pushScene.Push("sunPosition", sunPosition);
	pushScene.Push("displaySize", pipeline.GetRenderArea().GetExtent());
	pushScene.Push("worldHeight", sunHeight);

	// Updates descriptors.
	descriptorSet.Push("PushScene", pushScene);
	descriptorSet.Push("samplerMaterial", GetAttachment("samplerMaterial", "material"));
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!descriptorSet.Update(pipeline))
		return;

	// Draws the object.
	pipeline.BindPipeline(commandBuffer);

	descriptorSet.BindDescriptor(commandBuffer, pipeline);
	pushScene.BindPush(commandBuffer, pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

void FilterLensflare::SetSunPosition(const Vector3f &sunPosition) {
	auto camera = Scenes::Get()->GetCamera();
	this->sunPosition = Matrix4::Project(sunPosition, camera->GetViewMatrix(), camera->GetProjectionMatrix());
}
}
