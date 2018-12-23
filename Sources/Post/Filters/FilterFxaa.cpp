#include "FilterFxaa.hpp"

namespace acid
{
	FilterFxaa::FilterFxaa(const GraphicsStage &graphicsStage, const float &spanMax) :
		PostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Fxaa.frag"}, {}),
		m_pushScene(PushHandler()),
		m_spanMax(spanMax)
	{
	}

	void FilterFxaa::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
		// Updates uniforms.
		m_pushScene.Push("spanMax", m_spanMax);

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
		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
