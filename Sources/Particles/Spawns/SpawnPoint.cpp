#include "SpawnPoint.hpp"

namespace acid
{
	SpawnPoint::SpawnPoint() :
		ISpawnParticle(),
		m_point(Vector3())
	{
	}

	SpawnPoint::~SpawnPoint()
	{
	}

	Vector3 SpawnPoint::GetBaseSpawnPosition()
	{
		return m_point;
	}
}
