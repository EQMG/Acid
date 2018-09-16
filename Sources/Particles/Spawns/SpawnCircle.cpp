#include "SpawnCircle.hpp"

namespace acid
{
	SpawnCircle::SpawnCircle(const float &radius, const Vector3 &heading) :
		m_radius(radius),
		m_heading(heading.Normalize()),
		m_spawnPosition(Vector3())
	{
	}

	void SpawnCircle::Decode(const Metadata &metadata)
	{
		m_radius = metadata.GetChild<float>("Radius");
		m_heading = metadata.GetChild<Vector3>("Heading");
	}

	void SpawnCircle::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "SpawnCircle");
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<Vector3>("Heading", m_heading);
	}

	Vector3 SpawnCircle::GeneratePosition()
	{
		m_spawnPosition = Vector3::RandomPointOnCircle(m_heading, m_radius);
		return m_spawnPosition;
	}
}
