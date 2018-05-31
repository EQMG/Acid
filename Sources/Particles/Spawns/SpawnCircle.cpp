#include "SpawnCircle.hpp"

namespace fl
{
	SpawnCircle::SpawnCircle(const float &radius, const Vector3 &heading) :
		ISpawnParticle(),
		m_radius(radius),
		m_heading(heading.Normalize()),
		m_spawnPosition(Vector3())
	{
	}

	SpawnCircle::~SpawnCircle()
	{
	}

	Vector3 SpawnCircle::GetBaseSpawnPosition()
	{
		m_spawnPosition = Vector3::RandomPointOnCircle(m_heading, m_radius);
		return m_spawnPosition;
	}
}
