#include "waters.h"

namespace flounder
{
	waters::waters() :
		imodule()
	{
		m_water = NULL;
		m_colourIntensity = 0.4f; 
		m_enableReflections = true;
		m_reflectionQuality = 0.6f;
		m_reflectionShadows = true;
	}

	waters::~waters()
	{
		delete m_water;
	}

	void waters::update()
	{
		if (m_water != NULL)
		{
			m_water->update();
		}
	}
}
