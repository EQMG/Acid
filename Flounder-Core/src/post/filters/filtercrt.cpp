#include "filtercrt.h"

namespace flounder
{
	filtercrt::filtercrt(colour *screenColour, const float &curveAmountX, const float &curveAmountY, const float &scanLineSize, const float &scanIntensity) :
#ifdef FLOUNDER_PLATFORM_WEB
		ipostfilter("filterCrt", "res/shaders/filters/crtFragment.web.glsl", NULL)
#else
		ipostfilter("filterCrt", "res/shaders/filters/crtFragment.glsl")
#endif
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
		m_shader->loadUniform3f("screenColour", *m_screenColour);
		m_shader->loadUniform1f("curveAmountX", m_curveAmountX * static_cast<float>(display::get()->getAspectRatio()));
		m_shader->loadUniform1f("curveAmountY", m_curveAmountY);
		m_shader->loadUniform1f("scanLineSize", m_scanLineSize);
		m_shader->loadUniform1f("scanIntensity", m_scanIntensity);

		m_shader->loadUniform1f("moveTime", static_cast<float>(framework::get()->getTimeSec()) / 100.0f);
	}
}
