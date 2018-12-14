#include "EmitterCircle.hpp"

#include "Objects/Entity.hpp"

namespace acid
{
	EmitterCircle::EmitterCircle(const float &radius, const Vector3 &heading, const Transform &localTransform) :
		ParticleEmitter(localTransform),
		m_radius(radius),
		m_heading(heading.Normalize())
	{
	}

	void EmitterCircle::Start()
	{
	}

	void EmitterCircle::Update()
	{
	}

	void EmitterCircle::Decode(const Metadata &metadata)
	{
		m_radius = metadata.GetChild<float>("Radius");
		m_heading = metadata.GetChild<Vector3>("Heading");
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
	}

	void EmitterCircle::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<Vector3>("Heading", m_heading);
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
	}

	Vector3 EmitterCircle::GeneratePosition() const
	{
		auto spawnPosition = Vector3::RandomPointOnCircle(m_heading, m_radius);
		return spawnPosition;
	}
}
