#include "SpawnLine.hpp"

#include "Maths/Maths.hpp"

namespace acid
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

	void SpawnLine::Decode(const Node &node)
	{
		m_length = node.GetChild<float>("Length");
		m_axis = node.GetChild<Vector3>("Axis");
	}

	void SpawnLine::Encode(Node &node) const
	{
		node.SetChild<std::string>("Type", "SpawnLine");
		node.SetChild<float>("Length", m_length);
		node.SetChild<Vector3>("Axis", m_axis);
	}

	Vector3 SpawnLine::GetBaseSpawnPosition()
	{
		m_spawnPosition = m_axis;
		m_spawnPosition *= m_length;
		m_spawnPosition *= Maths::Random(-0.5f, 0.5f);
		return m_spawnPosition;
	}
}
