#include "Waters.hpp"

namespace Flounder
{
	Waters::Waters() :
		IModule(),
		m_colourIntensity(0.7f),
		m_enableReflections(false),
		m_reflectionQuality(0.6f),
		m_reflectionShadows(true)
	{
	}

	Waters::~Waters()
	{
	}

	void Waters::Update()
	{
	}
}
