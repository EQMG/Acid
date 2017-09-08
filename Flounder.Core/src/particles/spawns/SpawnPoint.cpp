#include "SpawnPoint.hpp"

namespace Flounder
{
	SpawnPoint::SpawnPoint() :
		ISpawnParticle(),
		m_point(new Vector3())
	{
	}

	SpawnPoint::~SpawnPoint()
	{
		delete m_point;
	}

	Vector3 *SpawnPoint::GetBaseSpawnPosition()
	{
		return m_point;
	}
}
