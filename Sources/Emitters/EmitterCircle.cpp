#include "EmitterCircle.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
EmitterCircle::EmitterCircle(const float& radius, const Vector3& heading, const Transform& localTransform) : Emitter(localTransform), m_radius(radius), m_heading(heading.Normalize()) {}

void EmitterCircle::Start() {}

void EmitterCircle::Update() {}

void EmitterCircle::Decode(const Metadata& metadata)
{
	metadata.GetChild("Radius", m_radius);
	metadata.GetChild("Heading", m_heading);
	metadata.GetChild("Local Transform", m_localTransform);
}

void EmitterCircle::Encode(Metadata& metadata) const
{
	metadata.SetChild("Radius", m_radius);
	metadata.SetChild("Heading", m_heading);
	metadata.SetChild("Local Transform", m_localTransform);
}

Vector3 EmitterCircle::GeneratePosition() const
{
	return Vector3::RandomPointOnCircle(m_heading, m_radius);
}
}
