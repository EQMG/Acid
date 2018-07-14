#include "Light.hpp"

namespace fl
{
	Light::Light(const Colour &colour, const float &radius, const Vector3 &offset) :
		IComponent(),
		m_colour(colour),
		m_position(Vector3()),
		m_offset(offset),
		m_radius(radius)
	{
	}

	Light::Light(const Light &source) :
		IComponent(),
		m_colour(source.m_colour),
		m_position(Vector3()),
		m_offset(source.m_offset),
		m_radius(source.m_radius)
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

	void Light::Load(LoadedValue *value)
	{
		m_colour = value->GetChild("Colour")->GetString();
		m_offset = value->GetChild("Offset");
		m_radius = value->GetChild("Radius")->Get<float>();
	}

	void Light::Write(LoadedValue *destination)
	{
		destination->GetChild("Colour", true)->SetString(m_colour.GetHex());
		m_offset.Write(destination->GetChild("Offset", true));
		destination->GetChild("Radius", true)->Set(m_radius);
	}

	Light &Light::operator=(const Light &other)
	{
		m_colour = other.m_colour;
		m_offset = other.m_offset;
		m_radius = other.m_radius;
		return *this;
	}

	Light &Light::operator=(LoadedValue *source)
	{
		m_colour = source->GetChild("colour");
		m_offset = source->GetChild("offset");
		m_radius = source->GetChild("radius")->Get<float>();
		return *this;
	}
}
