#include "filtergrain.h"

namespace flounder
{
	filtergrain::filtergrain(const float &strength) :
#ifdef FLOUNDER_API_WEB
		ipostfilter("filterGrain", "res/shaders/filters/grainFragment.web.glsl", NULL)
#else
		ipostfilter("filterGrain", "res/shaders/filters/grainFragment.glsl")
#endif
	{
		m_strength = strength;
	}

	filtergrain::~filtergrain()
	{
	}

	void filtergrain::storeValues()
	{
		m_shader->loadUniform1f("strength", m_strength);
	}
}
