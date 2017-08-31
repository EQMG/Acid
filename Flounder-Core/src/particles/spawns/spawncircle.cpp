#include "spawncircle.hpp"

namespace Flounder
{
	spawncircle::spawncircle(const float &radius, const vector3 &heading) :
		ispawnparticle(),
		m_radius(radius),
		m_heading(new vector3(heading)),
		m_spawnPosition(new vector3())
	{
		m_heading->normalize();
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
