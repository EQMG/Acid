#include "FilterBlur.hpp"

#include "Graphics/Graphics.hpp"

namespace acid {
FilterBlur::FilterBlur(const Pipeline::Stage &pipelineStage, const Vector2f &direction, const Type &type) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Blur.frag"}, GetDefines(type)),
	m_type(type),
	m_direction(direction) {
}

void FilterBlur::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	m_pushScene.Push("direction", m_direction);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!m_descriptorSet.Update(m_pipeline)) {
		return;
	}

	// Binds the pipeline.
	m_pipeline.BindPipeline(commandBuffer);

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	m_pushScene.BindPush(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}

std::vector<Shader::Define> FilterBlur::GetDefines(const Type &type) {
	std::vector<Shader::Define> defines;
	defines.emplace_back(Shader::Define("BLUR_TYPE", String::To(type)));
	return defines;
}
}
