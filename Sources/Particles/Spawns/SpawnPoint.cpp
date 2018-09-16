#include "SpawnPoint.hpp"

namespace acid
{
	SpawnPoint::SpawnPoint() :
		m_point(Vector3())
	{
	}

	void SpawnPoint::Decode(const Metadata &metadata)
	{
		m_point = metadata.GetChild<Vector3>("Point");
	}

	void SpawnPoint::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "SpawnPoint");
		metadata.SetChild<Vector3>("Point", m_point);
	}

	Vector3 SpawnPoint::GeneratePosition()
	{
		return m_point;
	}
}
