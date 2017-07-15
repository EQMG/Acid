#include "filtercrt.h"

namespace flounder {
	filtercrt::filtercrt(colour *screenColour, float curveAmountX, float curveAmountY, float scanLineSize, float scanIntensity) :
		ipostfilter("filterNegative", "Resources/shaders/filters/crtFragment.glsl")
	{
		m_screenColour = screenColour;
		m_curveAmountX = curveAmountX;
		m_curveAmountY = curveAmountY;
		m_scanLineSize = scanLineSize;
		m_scanIntensity = scanIntensity;
	}

	filtercrt::~filtercrt()
	{
		delete m_screenColour;
	}

	void filtercrt::storeValues()
	{
		m_shader->loadUniform("screenColour", m_screenColour);
		m_shader->loadUniform("curveAmountX", m_curveAmountX * (float) display::get()->getAspectRatio());
		m_shader->loadUniform("curveAmountY", m_curveAmountY);
		m_shader->loadUniform("scanLineSize", m_scanLineSize);
		m_shader->loadUniform("scanIntensity", m_scanIntensity);

		m_shader->loadUniform("moveTime", (float)framework::get()->getTimeSec() / 100.0f);
	}
}
