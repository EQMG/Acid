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
		m_shader->loadUniform1f("pixelSize", m_pixelSize);
		m_shader->loadUniform2f("displaySize", static_cast<float>(display::get()->getWidth()), static_cast<float>(display::get()->getHeight()));
	}
}
