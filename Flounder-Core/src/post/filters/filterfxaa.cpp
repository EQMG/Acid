#include "filterfxaa.h"

namespace flounder
{
	filterfxaa::filterfxaa(const float &spanMax) :
		ipostfilter("filterFxaa", "res/shaders/filters/fxaaFragment.glsl")
	{
		m_spanMax = spanMax;
	}

	filterfxaa::~filterfxaa()
	{
	}

	void filterfxaa::storeValues()
	{
		m_shader->loadUniform1f("spanMax", m_spanMax);
	}
}
