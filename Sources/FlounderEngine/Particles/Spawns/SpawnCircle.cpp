#include "SpawnCircle.hpp"

namespace Flounder
{
	SpawnCircle::SpawnCircle(const float &radius, const Vector3 &heading) :
		ISpawnParticle(),
		m_radius(radius),
		m_heading(new Vector3(heading)),
		m_spawnPosition(new Vector3())
	{
		m_heading->Normalize();
	}

	SpawnCircle::~SpawnCircle()
	{
		delete m_heading;
		delete m_spawnPosition;
	}

	Vector3 *SpawnCircle::GetBaseSpawnPosition()
	{
		*m_spawnPosition = Vector3::RandomPointOnCircle(*m_heading, m_radius);
		return m_spawnPosition;
	}
}
