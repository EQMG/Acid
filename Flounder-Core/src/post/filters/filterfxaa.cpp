#include "filterfxaa.h"

namespace flounder {
	filterfxaa::filterfxaa(const float &spanMax) :
		ipostfilter("filterFxaa", "res/shaders/filters/fxaaFragment.glsl")
	{
		m_spanMax = spanMax;
	}

	filterfxaa::filterfxaa() :
		filterfxaa(8.0f)
	{
	}

	filterfxaa::~filterfxaa()
	{
	}

	void filterfxaa::storeValues()
	{
		m_shader->loadUniform("spanMax", m_spanMax);
	}
}
