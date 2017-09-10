#include "FilterGrain.hpp"

namespace Flounder
{
	FilterGrain::FilterGrain(const float &strength) :
		IPostFilter("filterGrain", "res/shaders/filters/grain.frag.spv"),
		m_strength(strength)
	{
	}

	FilterGrain::~FilterGrain()
	{
	}

	void FilterGrain::StoreValues()
	{
#if 0
		m_shader->loadUniform1f("strength", m_strength);
#endif
	}
}
