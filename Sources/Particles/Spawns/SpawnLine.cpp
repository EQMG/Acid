#include "SpawnLine.hpp"

#include "Maths/Maths.hpp"

namespace fl
{
	SpawnLine::SpawnLine(const float &length, const Vector3 &axis) :
		ISpawnParticle(),
		m_length(length),
		m_axis(axis.Normalize()),
		m_spawnPosition(Vector3())
	{
	}

	SpawnLine::~SpawnLine()
	{
	}

	Vector3 SpawnLine::GetBaseSpawnPosition()
	{
		m_spawnPosition = m_axis;
		m_spawnPosition *= m_length;
		m_spawnPosition *= Maths::Random(-0.5f, 0.5f);
		return m_spawnPosition;
	}
}
