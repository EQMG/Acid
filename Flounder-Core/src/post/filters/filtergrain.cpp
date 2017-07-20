#include "filtergrain.h"

namespace flounder 
{
	filtergrain::filtergrain(const float &strength) :
		ipostfilter("filterGrain", "res/shaders/filters/grainFragment.glsl")
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
