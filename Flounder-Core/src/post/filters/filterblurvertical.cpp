#include "filterblurvertical.h"

namespace flounder
{
	filterblurvertical::filterblurvertical(const float &sizeScalar) :
		ipostfilter("filterBlurVertical", "res/shaders/filters/blurvertical.frag.spv", new fbo(true, sizeScalar))
	{
		m_heightValue = static_cast<int>(display::get()->getHeight() * sizeScalar);
		m_scaleValue = 2.0f;
		m_fitToDisplay = true;
		m_sizeScalar = sizeScalar;
	}

	filterblurvertical::filterblurvertical(const int &width, const int &height) :
		ipostfilter("filterBlurVertical", "res/shaders/filters/blurvertical.frag.spv", new fbo(width, height))
	{
		m_heightValue = height;
		m_scaleValue = 2.0f;
		m_fitToDisplay = false;
		m_sizeScalar = -1.0f;
	}

	filterblurvertical::~filterblurvertical()
	{
	}

	void filterblurvertical::storeValues()
	{
		if (m_fitToDisplay)
		{
			m_heightValue = static_cast<int>(display::get()->getHeight() * m_sizeScalar);
		}

		m_shader->loadUniform1f("height", static_cast<float>(m_heightValue));
		m_shader->loadUniform1f("scale", m_scaleValue);
	}
}
