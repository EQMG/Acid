#include "FilterDof.hpp"

#include "Graphics/Graphics.hpp"
#include "Scenes/Scenes.hpp"

namespace acid {
FilterDof::FilterDof(const Pipeline::Stage &pipelineStage, PipelineBlur *pipelineBlur, float focusPoint, float nearField, float nearTransition,
	float farField, float farTransition) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Dof.frag"}),
	pipelineBlur(pipelineBlur),
	focusPoint(focusPoint),
	nearField(nearField),
	nearTransition(nearTransition),
	farField(farField),
	farTransition(farTransition) {
}

void FilterDof::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	auto camera = Scenes::Get()->GetCamera();
	pushScene.Push("nearPlane", camera->GetNearPlane());
	pushScene.Push("farPlane", camera->GetFarPlane());
	pushScene.Push("focusPoint", focusPoint);
	pushScene.Push("nearField", nearField);
	pushScene.Push("nearTransition", nearTransition);
	pushScene.Push("farField", farField);
	pushScene.Push("farTransition", farTransition);

	// Updates descriptors.
	descriptorSet.Push("PushScene", pushScene);
	descriptorSet.Push("samplerDepth", Graphics::Get()->GetAttachment("depth"));
	descriptorSet.Push("samplerBlured", pipelineBlur ? pipelineBlur->GetOutput() : nullptr);
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
