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

	void Light::Decode(const Node &node)
	{
		m_colour = node.GetChild<Colour>("Colour");
		m_offset = node.GetChild<Vector3>("Offset");
		m_radius = node.GetChild<float>("Radius");
	}

	void Light::Encode(Node &node) const
	{
		node.SetChild<Colour>("Colour", m_colour);
		node.SetChild<Vector3>("Offset", m_offset);
		node.SetChild<float>("Radius", m_radius);
	}
}
