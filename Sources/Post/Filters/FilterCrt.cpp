#include "FilterCrt.hpp"

#include "Graphics/Graphics.hpp"

namespace acid
{
FilterCrt::FilterCrt(const Pipeline::Stage &pipelineStage, const Colour &screenColour, const float &curveAmountX, const float &curveAmountY, const float &scanLineSize,
	const float &scanIntensity) :
	PostFilter{pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Crt.frag"}},
	m_screenColour{screenColour},
	m_curveAmountX{curveAmountX},
	m_curveAmountY{curveAmountY},
	m_scanLineSize{scanLineSize},
	m_scanIntensity{scanIntensity}
{
}

void FilterCrt::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	m_pushScene.Push("screenColour", m_screenColour);
	m_pushScene.Push("curveAmountX", m_curveAmountX * m_pipeline.GetRenderArea().GetAspectRatio());
	m_pushScene.Push("curveAmountY", m_curveAmountY);
	m_pushScene.Push("scanLineSize", m_scanLineSize);
	m_pushScene.Push("scanIntensity", m_scanIntensity);
	m_pushScene.Push("moveTime", Time::Now() / 100.0f);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
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
