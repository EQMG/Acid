#include "filtergrain.hpp"

namespace flounder
{
	filtergrain::filtergrain(const float &strength) :
		ipostfilter("filterGrain", "res/shaders/filters/grain.frag.spv"),
		m_strength(strength)
	{
	}

	filtergrain::~filtergrain()
	{
	}

	void filtergrain::storeValues()
	{
#if 0
		m_shader->loadUniform1f("strength", m_strength);
#endif
	}
}
