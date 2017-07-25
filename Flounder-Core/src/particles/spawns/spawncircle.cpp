#include "spawncircle.h"

namespace flounder
{
	spawncircle::spawncircle(const float &radius, const vector3 &heading) :
		ispawnparticle()
	{
		m_radius = radius;
		m_heading = new vector3(heading);
		m_heading->normalize();
		m_spawnPosition = new vector3();
	}

	spawncircle::~spawncircle()
	{
		delete m_heading;
		delete m_spawnPosition;
	}

	vector3 *spawncircle::getBaseSpawnPosition()
	{
		return vector3::randomPointOnCircle(*m_heading, m_radius, m_spawnPosition);
	}
}
