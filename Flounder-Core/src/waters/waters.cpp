#include "waters.hpp"

namespace flounder
{
	waters::waters() :
		imodule()
	{
		m_water = nullptr;
		m_colourIntensity = 0.7f;
		m_enableReflections = false;
		m_reflectionQuality = 0.6f;
		m_reflectionShadows = true;
	}

	waters::~waters()
	{
		delete m_water;
	}

	void waters::update()
	{
		if (m_water != nullptr)
		{
			m_water->update();
		}
	}
}
