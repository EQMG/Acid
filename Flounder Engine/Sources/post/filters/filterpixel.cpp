#include "filterpixel.h"

namespace flounder {
	filterpixel::filterpixel(const float &pixelSize) :
		ipostfilter("filterGrain", "Resources/shaders/filters/grainFragment.glsl")
	{
		m_pixelSize = pixelSize;
	}

	filterpixel::filterpixel() :
		filterpixel(2.0f)
	{
	}

	filterpixel::~filterpixel()
	{
	}

	void filterpixel::storeValues()
	{
		m_shader->loadUniform("pixelSize", m_pixelSize);
		m_shader->loadUniform("displaySize", display::get()->getWidth(), display::get()->getHeight());
	}
}
