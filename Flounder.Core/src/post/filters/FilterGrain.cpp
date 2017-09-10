#include "filtergrain.hpp"

namespace Flounder
{
	filtergrain::filtergrain(const float &strength) :
		IPostFilter("filterGrain", "res/shaders/filters/grain.frag.spv"),
		m_strength(strength)
	{
	}

	filtergrain::~filtergrain()
	{
	}

	void filtergrain::StoreValues()
	{
#if 0
		m_shader->loadUniform1f("strength", m_strength);
#endif
	}
}
