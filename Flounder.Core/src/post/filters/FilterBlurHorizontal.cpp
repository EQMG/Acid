#include "filterblurhorizontal.hpp"

namespace Flounder
{
	filterblurhorizontal::filterblurhorizontal(const float &sizeScalar) :
		IPostFilter("filterBlurHorizontal", "res/shaders/filters/blurhorizontal.frag.spv", new Fbo(true, sizeScalar)),
		m_widthValue(static_cast<int>(Display::Get()->GetWidth() * sizeScalar)),
		m_scaleValue(2.0f),
		m_fitToDisplay(true),
		m_sizeScalar(sizeScalar)
	{
	}

	filterblurhorizontal::filterblurhorizontal(const int &width, const int &height) :
		IPostFilter("filterBlurHorizontal", "res/shaders/filters/blurhorizontal.frag.spv", new Fbo(width, height)),
		m_widthValue(width),
		m_scaleValue(2.0f),
		m_fitToDisplay(false),
		m_sizeScalar(-1.0f)
	{
	}

	filterblurhorizontal::~filterblurhorizontal()
	{
	}

	void filterblurhorizontal::StoreValues()
	{
		if (m_fitToDisplay)
		{
			m_widthValue = static_cast<int>(Display::Get()->GetWidth() * m_sizeScalar);
		}
#if 0
		m_shader->loadUniform1f("width", static_cast<float>(m_widthValue));
		m_shader->loadUniform1f("scale", m_scaleValue);
#endif
	}
}
