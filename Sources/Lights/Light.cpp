#include "Light.hpp"

namespace acid
{
	Light::Light(const Colour &colour, const float &radius, const Vector3 &offset) :
		m_colour(colour),
		m_offset(offset),
		m_radius(radius),
		m_position(Vector3())
	{
	}

	Light::Light(const Light &source) :
		m_colour(source.m_colour),
		m_offset(source.m_offset),
		m_radius(source.m_radius),
		m_position(Vector3())
	{
	}

	Light::~Light()
	{
	}

	void Light::Start()
	{
	}

	void Light::Update()
	{
		m_position = GetGameObject()->GetTransform().GetPosition() + m_offset;
	}

	void Light::Decode(const Metadata &metadata)
	{
		m_colour = metadata.GetChild<Colour>("Colour");
		m_offset = metadata.GetChild<Vector3>("Offset");
		m_radius = metadata.GetChild<float>("Radius");
	}

	void Light::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Colour>("Colour", m_colour);
		metadata.SetChild<Vector3>("Offset", m_offset);
		metadata.SetChild<float>("Radius", m_radius);
	}
}
