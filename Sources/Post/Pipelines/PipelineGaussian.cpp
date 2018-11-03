#include "PipelineGaussian.hpp"

#include "Display/Display.hpp"

namespace acid
{
	PipelineGaussian::PipelineGaussian(const GraphicsStage &graphicsStage, const float &scale, const BlurType &blurType) :
		IPostPipeline(graphicsStage),
		m_filterBlurVertical(FilterBlur(graphicsStage, Vector2(0.0f, scale), blurType)),
		m_filterBlurHorizontal(FilterBlur(graphicsStage, Vector2(scale, 0.0f), blurType))
	//	m_halfRes(nullptr),
	//	m_lastWidth(0),
	//	m_lastHeight(0)
	{
	}

	void PipelineGaussian::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		/*uint32_t width = Display::Get()->GetWidth();
		uint32_t height = Display::Get()->GetHeight();

		if (width != m_lastWidth || height != m_lastHeight)
		{
			m_halfRes = std::make_unique<Texture>(width / 2, height / 2, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

			m_lastWidth = width;
			m_lastHeight = height;
		}

		Texture::CopyImage(m_filterBlurVertical.GetPipeline().GetTexture(5)->GetImage(), m_halfRes->GetImage(), m_halfRes->GetBufferMemory(), width / 2, height / 2, false);*/

		m_filterBlurVertical.Render(commandBuffer, clipPlane, camera);
		m_filterBlurHorizontal.Render(commandBuffer, clipPlane, camera);
	}
}
