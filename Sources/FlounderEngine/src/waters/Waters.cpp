#include "Waters.hpp"

namespace Flounder
{
	Waters::Waters() :
		IModule(),
		m_water(nullptr),
		m_colourIntensity(0.7f),
		m_enableReflections(false),
		m_reflectionQuality(0.6f),
		m_reflectionShadows(true)
	{
	}

	Waters::~Waters()
	{
		delete m_water;
	}

	void Waters::Update()
	{
		if (m_water != nullptr)
		{
			m_water->Update();
		}
	}
}
