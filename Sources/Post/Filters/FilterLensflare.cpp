#include "FilterLensflare.hpp"

#include "Scenes/Scenes.hpp"

namespace acid
{
FilterLensflare::FilterLensflare(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, { "Shaders/Post/Default.vert", "Shaders/Post/Lensflare.frag" }, {}),
	m_sunHeight(0.0f)
{
}

void FilterLensflare::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	m_pushScene.Push("sunPosition", m_sunPosition);
	m_pushScene.Push("displaySize", m_pipeline.GetSize());
	m_pushScene.Push("worldHeight", m_sunHeight);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
	//m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
	m_descriptorSet.Push("samplerMaterial", GetAttachment("samplerMaterial", "material"));
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
	bool updateSuccess = m_descriptorSet.Update(m_pipeline);

	if (!updateSuccess)
	{
		return;
	}

	// Draws the object.
	m_pushScene.BindPush(commandBuffer, m_pipeline);
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer.GetCommandBuffer(), 3, 1, 0, 0);
}

void FilterLensflare::SetSunPosition(const Vector3f &sunPosition)
{
	auto camera = Scenes::Get()->GetCamera();
	m_sunPosition = Matrix4::Project(sunPosition, camera->GetViewMatrix(), camera->GetProjectionMatrix());
}
}
