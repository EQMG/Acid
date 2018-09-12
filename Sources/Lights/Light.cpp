#include "Light.hpp"

#include "Objects/GameObject.hpp"

namespace acid
{
	Light::Light(const Colour &colour, const float &radius, const Vector3 &offset) :
		m_colour(colour),
		m_radius(radius),
		m_position(Vector3()),
		m_localOffset(offset)
	{
	}

	Light::Light(const Light &source) :
		m_colour(source.m_colour),
		m_radius(source.m_radius),
		m_position(Vector3()),
		m_localOffset(source.m_localOffset)
	{
	}

	void Light::Start()
	{
	}

	void Light::Update()
	{
		m_position = GetGameObject()->GetTransform().GetPosition() + m_localOffset;
	}

	void Light::Decode(const Metadata &metadata)
	{
		m_colour = metadata.GetChild<Colour>("Colour");
		m_radius = metadata.GetChild<float>("Radius");
		m_localOffset = metadata.GetChild<Vector3>("Local Offset");
	}

	void Light::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Colour>("Colour", m_colour);
		metadata.SetChild<float>("Radius", m_radius);
		metadata.SetChild<Vector3>("Local Offset", m_localOffset);
	}
}
