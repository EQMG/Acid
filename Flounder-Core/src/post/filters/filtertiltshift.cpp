#include "filtertiltshift.h"

namespace flounder
{
	filtertiltshift::filtertiltshift(const float &blurAmount, const float &centre, const float &stepSize, const float &steps) :
		ipostfilter("filterTiltshift", "res/shaders/filters/tiltshiftFragment.glsl")
	{
		m_blurAmount = blurAmount;
		m_centre = centre;
		m_stepSize = stepSize;
		m_steps = steps;
	}

	filtertiltshift::filtertiltshift() :
		filtertiltshift(1.0f, 1.1f, 0.004f, 3.0f)
	{
	}

	filtertiltshift::~filtertiltshift()
	{
	}

	void filtertiltshift::storeValues()
	{
		m_shader->loadUniform("blurAmount", m_blurAmount);
		m_shader->loadUniform("centre", m_centre);
		m_shader->loadUniform("stepSize", m_stepSize);
		m_shader->loadUniform("steps", m_steps);
	}
}
