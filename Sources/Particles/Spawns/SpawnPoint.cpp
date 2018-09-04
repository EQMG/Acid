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

	void SpawnPoint::Decode(const Node &node)
	{
		m_point = node.GetChild<Vector3>("Point");
	}

	void SpawnPoint::Encode(Node &node) const
	{
		node.SetChild<std::string>("Type", "SpawnPoint");
		node.SetChild<Vector3>("Point", m_point);
	}

	Vector3 SpawnPoint::GetBaseSpawnPosition()
	{
		return m_point;
	}
}
