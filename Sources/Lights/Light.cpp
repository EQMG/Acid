#include "Light.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
Light::Light(const Colour& colour, const float& radius, const Transform& localTransform) : m_colour(colour), m_radius(radius), m_localTransform(localTransform) {}

void Light::Start() {}

void Light::Update() {}

void Light::Decode(const Metadata& metadata)
{
	metadata.GetChild("Colour", m_colour);
	metadata.GetChild("Radius", m_radius);
	metadata.GetChild("Local Transform", m_localTransform);
}

void Light::Encode(Metadata& metadata) const
{
	metadata.SetChild("Colour", m_colour);
	metadata.SetChild("Radius", m_radius);
	metadata.SetChild("Local Transform", m_localTransform);
}

Transform Light::GetWorldTransform() const
{
	if(m_localTransform.IsDirty() || GetParent()->GetWorldTransform().IsDirty())
		{
			m_worldTransform = GetParent()->GetWorldTransform() * m_localTransform;
			m_localTransform.SetDirty(false);
		}

	return m_worldTransform;
}
}
