#include "filtergrain.h"

namespace flounder
{
	filtergrain::filtergrain(const float &strength) :
#ifdef FLOUNDER_PLATFORM_WEB
		ipostfilter("filterGrain", "res/shaders/filters/grainFragment.web.glsl", NULL)
#else
		ipostfilter("filterGrain", "res/shaders/filters/grainFragment.glsl")
#endif
	{
		m_strength = strength;
	}

	filtergrain::filtergrain() :
		filtergrain(2.3f)
	{
	}

	filtergrain::~filtergrain()
	{
	}

	void filtergrain::storeValues()
	{
		m_shader->loadUniform("strength", m_strength);
	}
}
