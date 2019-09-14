#include "FilterDof.hpp"

#include "Graphics/Graphics.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
FilterDof::FilterDof(const Pipeline::Stage &pipelineStage, PipelineBlur *pipelineBlur, float focusPoint, float nearField, float nearTransition,
	float farField, float farTransition) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Dof.frag"}),
	m_pipelineBlur(pipelineBlur),
	m_focusPoint(focusPoint),
	m_nearField(nearField),
	m_nearTransition(nearTransition),
	m_farField(farField),
	m_farTransition(farTransition)
{
}

void FilterDof::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	auto camera = Scenes::Get()->GetCamera();
	m_pushScene.Push("nearPlane", camera->GetNearPlane());
	m_pushScene.Push("farPlane", camera->GetFarPlane());
	m_pushScene.Push("focusPoint", m_focusPoint);
	m_pushScene.Push("nearField", m_nearField);
	m_pushScene.Push("nearTransition", m_nearTransition);
	m_pushScene.Push("farField", m_farField);
	m_pushScene.Push("farTransition", m_farTransition);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
	m_descriptorSet.Push("samplerDepth", Graphics::Get()->GetAttachment("depth"));
	m_descriptorSet.Push("samplerBlured", m_pipelineBlur ? m_pipelineBlur->GetOutput() : nullptr);
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!m_descriptorSet.Update(m_pipeline))
	{
		return;
	}

	// Draws the object.
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	m_pushScene.BindPush(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
