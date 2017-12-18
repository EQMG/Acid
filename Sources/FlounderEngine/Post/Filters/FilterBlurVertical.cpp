#include "FilterBlurVertical.hpp"

#include "../../Devices/Display.hpp"

namespace Flounder
{
	FilterBlurVertical::FilterBlurVertical(const int &subpass, const float &sizeScalar) :
		IPostFilter("Resources/Shaders/Filters/BlurVertical.frag.spv", subpass, {}),
		m_heightValue(static_cast<int>(Display::Get()->GetHeight() * sizeScalar)),
		m_scaleValue(2.0f),
		m_fitToDisplay(true),
		m_sizeScalar(sizeScalar)
	{
	}

	FilterBlurVertical::FilterBlurVertical(const int &subpass, const int &width, const int &height) :
		IPostFilter("Resources/Shaders/Filters/BlurVertical.frag.spv", subpass, {}),
		m_heightValue(height),
		m_scaleValue(2.0f),
		m_fitToDisplay(false),
		m_sizeScalar(-1.0f)
	{
	}

	FilterBlurVertical::~FilterBlurVertical()
	{
	}

	void FilterBlurVertical::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		if (m_fitToDisplay)
		{
			m_heightValue = static_cast<int>(Display::Get()->GetHeight() * m_sizeScalar);
		}
#if 0
		m_shader->loadUniform1f("height", static_cast<float>(m_heightValue));
		m_shader->loadUniform1f("scale", m_scaleValue);
#endif
	}
}
