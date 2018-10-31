#include "FilterCrt.hpp"

#include "Display/Display.hpp"

namespace acid
{
	FilterCrt::FilterCrt(const GraphicsStage &graphicsStage, const Colour &screenColour, const float &curveAmountX, const float &curveAmountY, const float &scanLineSize, const float &scanIntensity) :
		IPostFilter(graphicsStage, {"Shaders/Filters/Default.vert", "Shaders/Filters/Crt.frag"}, {}),
		m_uniformScene(UniformHandler()),
		m_screenColour(screenColour),
		m_curveAmountX(curveAmountX),
		m_curveAmountY(curveAmountY),
		m_scanLineSize(scanLineSize),
		m_scanIntensity(scanIntensity)
	{
	}

	void FilterCrt::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		// Updates uniforms.
		m_uniformScene.Push("screenColour", m_screenColour);
		m_uniformScene.Push("curveAmountX", m_curveAmountX * Display::Get()->GetAspectRatio());
		m_uniformScene.Push("curveAmountY", m_curveAmountY);
		m_uniformScene.Push("scanLineSize", m_scanLineSize);
		m_uniformScene.Push("scanIntensity", m_scanIntensity);
		m_uniformScene.Push("moveTime", Engine::GetTime() / 100.0f);

		// Updates descriptors.
		m_descriptorSet.Push("UboScene", &m_uniformScene);
		m_descriptorSet.Push("writeColour", Renderer::Get()->GetAttachment("resolved"));
		m_descriptorSet.Push("samplerColour", Renderer::Get()->GetAttachment("resolved"));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
