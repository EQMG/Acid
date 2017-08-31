#include "waters.hpp"

namespace Flounder
{
	waters::waters() :
		IModule(),
		m_water(nullptr),
		m_colourIntensity(0.7f),
		m_enableReflections(false),
		m_reflectionQuality(0.6f),
		m_reflectionShadows(true)
	{
	}

	waters::~waters()
	{
		delete m_water;
	}

	void waters::Update()
	{
		if (m_water != nullptr)
		{
			m_water->update();
		}
	}
}
