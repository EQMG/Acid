#include "SpawnCircle.hpp"

namespace acid
{
	SpawnCircle::SpawnCircle(const float &radius, const Vector3 &heading) :
		m_radius(radius),
		m_heading(heading.Normalize()),
		m_spawnPosition(Vector3())
	{
	}

	SpawnCircle::~SpawnCircle()
	{
	}

	void SpawnCircle::Decode(const Node &node)
	{
		m_radius = node.GetChild<float>("Radius");
		m_heading = node.GetChild<Vector3>("Heading");
	}

	void SpawnCircle::Encode(Node &node) const
	{
		node.SetChild<std::string>("Type", "SpawnCircle");
		node.SetChild<float>("Radius", m_radius);
		node.SetChild<Vector3>("Heading", m_heading);
	}

	Vector3 SpawnCircle::GetBaseSpawnPosition()
	{
		m_spawnPosition = Vector3::RandomPointOnCircle(m_heading, m_radius);
		return m_spawnPosition;
	}
}
