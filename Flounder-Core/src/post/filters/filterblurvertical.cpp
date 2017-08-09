#include "filterblurvertical.h"

namespace flounder
{
	filterblurvertical::filterblurvertical(const float &sizeScalar) :
#ifdef FLOUNDER_API_WEB
		ipostfilter("filterBlurVertical", "res/shaders/filters/blurverticalFragment.web.glsl", fbo::newFBO()->fitToScreen(sizeScalar)->disableTextureWrap()->create())
#else
		ipostfilter("filterBlurVertical", "res/shaders/filters/blurverticalFragment.glsl", fbo::newFBO()->fitToScreen(sizeScalar)->disableTextureWrap()->create())
#endif
	{
		m_heightValue = static_cast<int>(display::get()->getHeight() * sizeScalar);
		m_scaleValue = 2.0f;
		m_fitToDisplay = true;
		m_sizeScalar = sizeScalar;
	}

	filterblurvertical::filterblurvertical(const int &width, const int &height) :
#ifdef FLOUNDER_API_WEB
		ipostfilter("filterBlurVertical", "res/shaders/filters/blurverticalFragment.web.glsl", fbo::newFBO()->setSize(width, height)->disableTextureWrap()->create())
#else
		ipostfilter("filterBlurVertical", "res/shaders/filters/blurverticalFragment.glsl", fbo::newFBO()->setSize(width, height)->disableTextureWrap()->create())
#endif
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
