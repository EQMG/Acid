#include "Light.hpp"

#include "Scenes/Entity.hpp"

namespace acid
{
Light::Light(const Colour &colour, const float &radius, const Transform &localTransform) :
	m_colour(colour),
	m_radius(radius),
	m_localTransform(localTransform)
{
}

void Light::Start()
{
}

void Light::Update()
{
}

Transform Light::GetWorldTransform() const
{
	if (m_localTransform.IsDirty() || GetParent()->GetWorldTransform().IsDirty())
	{
		m_worldTransform = GetParent()->GetWorldTransform() * m_localTransform;
		m_localTransform.SetDirty(false);
	}

	return m_worldTransform;
}

const Metadata &operator>>(const Metadata &metadata, Light &light)
{
	metadata.GetChild("Colour", light.m_colour);
	metadata.GetChild("Radius", light.m_radius);
	metadata.GetChild("Local Transform", light.m_localTransform);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Light &light)
{
	metadata.SetChild("Colour", light.m_colour);
	metadata.SetChild("Radius", light.m_radius);
	metadata.SetChild("Local Transform", light.m_localTransform);
	return metadata;
}
}
