#include "PipelineBlur.hpp"

#include "Devices/Window.hpp"
#include "Graphics/Graphics.hpp"

namespace acid
{
PipelineBlur::PipelineBlur(const Pipeline::Stage &pipelineStage, const float &blur, const FilterBlur::Type &blurType, const bool &toScreen, const float &inputScale,
	const float &outputScale) :
	PostPipeline{pipelineStage},
	m_filterBlurVertical{pipelineStage, {0.0f, blur}, blurType},
	m_filterBlurHorizontal{pipelineStage, {blur, 0.0f}, blurType},
	m_toScreen{toScreen},
	m_inputScale{inputScale},
	m_outputScale{outputScale},
	m_blur{blur}
{
}

void PipelineBlur::Render(const CommandBuffer &commandBuffer)
{
	if (!m_toScreen)
	{
		auto size = Window::Get()->GetSize();

		if (size != m_lastSize)
		{
			auto newSize = m_outputScale * size;
			m_output = std::make_unique<Image2d>(newSize, nullptr, VK_FORMAT_R8G8B8A8_UNORM);

			m_filterBlurVertical.SetAttachment("writeColour", m_output.get());
			m_filterBlurHorizontal.SetAttachment("writeColour", m_output.get());

			m_lastSize = size;
		}
	}

	// Input might be scaled for faster blur.
	//Image2d::CopyImage(dynamic_cast<Image2d *>(Renderer::Get()->GetAttachment("resolved"))->GetImage(), m_halfRes->GetImage(), m_halfRes->GetDeviceMemory(), width / 2, height / 2, false, 0, 1);

	m_filterBlurVertical.SetDirection({0.0f, m_blur});
	m_filterBlurHorizontal.SetDirection({m_blur, 0.0f});

	m_filterBlurVertical.Render(commandBuffer);
	m_filterBlurHorizontal.Render(commandBuffer);
}
}
