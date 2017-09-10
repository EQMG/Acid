#include "filterpixel.hpp"

namespace Flounder
{
	filterpixel::filterpixel(const float &pixelSize) :
		IPostFilter("filterGrain", "res/shaders/filters/grain.frag.spv"),
		m_pixelSize(pixelSize)
	{
	}

	filterpixel::~filterpixel()
	{
	}

	void filterpixel::StoreValues()
	{
#if 0
		m_shader->loadUniform1f("pixelSize", m_pixelSize);
		m_shader->loadUniform2f("displaySize", static_cast<float>(Display::Get()->getWidth()), static_cast<float>(Display::Get()->getHeight()));
#endif
	}
}
