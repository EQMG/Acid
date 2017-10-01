#include "FilterTiltshift.hpp"

#include "../../devices/Display.hpp"

namespace Flounder
{
	FilterTiltshift::FilterTiltshift(const float &blurAmount, const float &centre, const float &stepSize, const float &steps) :
		IPostFilter("filterTiltshift", "res/shaders/filters/tiltshift.frag.spv"),
		m_blurAmount(blurAmount),
		m_centre(centre),
		m_stepSize(stepSize),
		m_steps(steps)
	{
	}

	FilterTiltshift::~FilterTiltshift()
	{
	}

	void FilterTiltshift::StoreValues()
	{
#if 0
		m_shader->loadUniform1f("blurAmount", m_blurAmount);
		m_shader->loadUniform1f("centre", m_centre);
		m_shader->loadUniform1f("stepSize", m_stepSize);
		m_shader->loadUniform1f("steps", m_steps);
#endif
	}
}
