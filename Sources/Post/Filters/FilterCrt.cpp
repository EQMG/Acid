#include "FilterCrt.hpp"

#include "Display/Display.hpp"

namespace acid
{
	FilterCrt::FilterCrt(const GraphicsStage &graphicsStage, const Colour &screenColour, const float &curveAmountX, const float &curveAmountY, const float &scanLineSize, const float &scanIntensity) :
		PostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Crt.frag"}, {}),
		m_pushScene(PushHandler()),
		m_screenColour(screenColour),
		m_curveAmountX(curveAmountX),
		m_curveAmountY(curveAmountY),
		m_scanLineSize(scanLineSize),
		m_scanIntensity(scanIntensity)
	{
	}

	void FilterCrt::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
		// Updates uniforms.
		m_pushScene.Push("screenColour", m_screenColour);
		m_pushScene.Push("curveAmountX", m_curveAmountX * Display::Get()->GetAspectRatio());
		m_pushScene.Push("curveAmountY", m_curveAmountY);
		m_pushScene.Push("scanLineSize", m_scanLineSize);
		m_pushScene.Push("scanIntensity", m_scanIntensity);
		m_pushScene.Push("moveTime", Engine::GetTime() / 100.0f);

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

		// Draws the object.
		m_pushScene.BindPush(commandBuffer, m_pipeline);
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
