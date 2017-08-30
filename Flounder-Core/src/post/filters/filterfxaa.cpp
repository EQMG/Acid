#include "filterfxaa.hpp"

namespace flounder
{
	filterfxaa::filterfxaa(const float &spanMax) :
		ipostfilter("filterFxaa", "res/shaders/filters/fxaa.frag.spv"),
		m_spanMax(spanMax)
	{
	}

	filterfxaa::~filterfxaa()
	{
	}

	void filterfxaa::storeValues()
	{
#if 0
		m_shader->loadUniform1f("spanMax", m_spanMax);
#endif
	}
}
