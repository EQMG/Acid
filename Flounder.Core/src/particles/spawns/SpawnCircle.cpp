#include "spawncircle.hpp"

namespace Flounder
{
	spawncircle::spawncircle(const float &radius, const Vector3 &heading) :
		ispawnparticle(),
		m_radius(radius),
		m_heading(new Vector3(heading)),
		m_spawnPosition(new Vector3())
	{
		m_heading->normalize();
	}

	spawncircle::~spawncircle()
	{
		delete m_heading;
		delete m_spawnPosition;
	}

	Vector3 *spawncircle::getBaseSpawnPosition()
	{
		return Vector3::randomPointOnCircle(*m_heading, m_radius, m_spawnPosition);
	}
}
