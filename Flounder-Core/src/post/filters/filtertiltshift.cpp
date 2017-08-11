#include "filtertiltshift.h"

namespace flounder
{
	filtertiltshift::filtertiltshift(const float &blurAmount, const float &centre, const float &stepSize, const float &steps) :
		ipostfilter("filterTiltshift", "res/shaders/filters/tiltshift.frag.spv")
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
		m_shader->loadUniform1f("blurAmount", m_blurAmount);
		m_shader->loadUniform1f("centre", m_centre);
		m_shader->loadUniform1f("stepSize", m_stepSize);
		m_shader->loadUniform1f("steps", m_steps);
	}
}
