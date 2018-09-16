#include "SpawnLine.hpp"

#include "Maths/Maths.hpp"

namespace acid
{
	SpawnLine::SpawnLine(const float &length, const Vector3 &axis) :
		m_length(length),
		m_axis(axis.Normalize()),
		m_spawnPosition(Vector3())
	{
	}

	void SpawnLine::Decode(const Metadata &metadata)
	{
		m_length = metadata.GetChild<float>("Length");
		m_axis = metadata.GetChild<Vector3>("Axis");
	}

	void SpawnLine::Encode(Metadata &metadata) const
	{
		metadata.SetChild<std::string>("Type", "SpawnLine");
		metadata.SetChild<float>("Length", m_length);
		metadata.SetChild<Vector3>("Axis", m_axis);
	}

	Vector3 SpawnLine::GeneratePosition()
	{
		m_spawnPosition = m_axis;
		m_spawnPosition *= m_length;
		m_spawnPosition *= Maths::Random(-0.5f, 0.5f);
		return m_spawnPosition;
	}
}
