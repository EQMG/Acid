#include "FilterBlur.hpp"

#include "Renderer/Renderer.hpp"

namespace acid
{
	FilterBlur::FilterBlur(const GraphicsStage &graphicsStage, const Vector2 &direction, const Type &type) :
		PostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Blur.frag"}, {ShaderDefine("BLUR_TYPE", String::To(static_cast<uint32_t>(type)))}),
		m_pushScene(PushHandler()),
		m_type(type),
		m_direction(direction)
	{
	}

	void FilterBlur::Render(const CommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_pushScene.Push("direction", m_direction);

		// Updates descriptors.
		m_descriptorSet.Push("PushScene", m_pushScene);
	//	m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//	m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
		PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Binds the pipeline.
		m_pushScene.BindPush(commandBuffer, m_pipeline);
		m_pipeline.BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		m_model->CmdRender(commandBuffer);
	}
}
