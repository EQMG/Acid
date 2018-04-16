#include "SpawnLine.hpp"

#include "Maths/Maths.hpp"

namespace Flounder
{
	SpawnLine::SpawnLine(const float &length, const Vector3 &axis) :
		ISpawnParticle(),
		m_length(length),
		m_axis(new Vector3(axis)),
		m_spawnPosition(new Vector3())
	{
		m_axis->Normalize();
	}

	SpawnLine::~SpawnLine()
	{
		delete m_axis;
		delete m_spawnPosition;
	}

	Vector3 *SpawnLine::GetBaseSpawnPosition()
	{
		*m_spawnPosition = *m_axis;
		*m_spawnPosition *= m_length;
		*m_spawnPosition *= Maths::RandomInRange(-0.5f, 0.5f);
		return m_spawnPosition;
	}
}
