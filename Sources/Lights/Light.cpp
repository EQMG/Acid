#include "Light.hpp"

#include "Objects/Entity.hpp"

namespace acid
{
	Light::Light(const Colour &colour, const float &radius, const Transform &localTransform) :
		m_colour(colour),
		m_radius(radius),
		m_position(Vector3()),
		m_localTransform(localTransform),
		m_worldTransform(Transform())
	{
	}

	Light::Light(const Light &source) :
		m_colour(source.m_colour),
		m_radius(source.m_radius),
		m_position(Vector3()),
		m_localTransform(source.m_localTransform),
		m_worldTransform(source.m_worldTransform)
	{
	}

	void Light::Start()
	{
	}

	void Light::Update()
	{
	}

	void Light::Decode(const Metadata &metadata)
	{
		m_colour = metadata.GetChild<Colour>("Colour");
		m_radius = metadata.GetChild<float>("Radius");
		m_localTransform = metadata.GetChild<Transform>("Local Transform");
	}

	void Light::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Colour>("Colour", m_colour);
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<Transform>("Local Transform", m_localTransform);
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
}
