#include "filterpixel.h"

namespace flounder
{
	filterpixel::filterpixel(const float &pixelSize) :
		ipostfilter("filterGrain", "res/shaders/filters/grainFragment.glsl")
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
		m_shader->loadUniform("displaySize", (float) display::get()->getWidth(), (float) display::get()->getHeight());
	}
}
