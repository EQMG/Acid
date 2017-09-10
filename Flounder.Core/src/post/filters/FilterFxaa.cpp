#include "filterfxaa.hpp"

namespace Flounder
{
	filterfxaa::filterfxaa(const float &spanMax) :
		IPostFilter("filterFxaa", "res/shaders/filters/fxaa.frag.spv"),
		m_spanMax(spanMax)
	{
	}

	filterfxaa::~filterfxaa()
	{
	}

	void filterfxaa::StoreValues()
	{
#if 0
		m_shader->loadUniform1f("spanMax", m_spanMax);
#endif
	}
}
