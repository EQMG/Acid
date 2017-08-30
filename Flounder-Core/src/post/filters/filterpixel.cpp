#include "filterpixel.hpp"

namespace flounder
{
	filterpixel::filterpixel(const float &pixelSize) :
		ipostfilter("filterGrain", "res/shaders/filters/grain.frag.spv"),
		m_pixelSize(pixelSize)
	{
	}

	filterpixel::~filterpixel()
	{
	}

	void filterpixel::storeValues()
	{
#if 0
		m_shader->loadUniform1f("pixelSize", m_pixelSize);
		m_shader->loadUniform2f("displaySize", static_cast<float>(display::get()->getWidth()), static_cast<float>(display::get()->getHeight()));
#endif
	}
}
