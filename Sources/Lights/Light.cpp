#include "Light.hpp"

namespace acid
{
	Light::Light(const Colour &colour, const float &radius, const Vector3 &offset) :
		IComponent(),
		m_colour(colour),
		m_offset(offset),
		m_radius(radius),
		m_position(Vector3())
	{
	}

	Light::Light(const Light &source) :
		IComponent(),
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

	void Light::Decode(const Serialized &serialized)
	{
		m_colour = serialized.GetChild<Colour>("Colour");
		m_offset = serialized.GetChild<Vector3>("Offset");
		m_radius = serialized.GetChild<float>("Radius");
	}

	void Light::Encode(Serialized &serialized) const
	{
		serialized.SetChild<Colour>("Colour", m_colour);
		serialized.SetChild<Vector3>("Offset", m_offset);
		serialized.SetChild<float>("Radius", m_radius);
	}
}
