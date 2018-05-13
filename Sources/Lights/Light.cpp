#include "Light.hpp"

namespace fl
{
	Light::Light(const Colour &colour, const float &radius, const Vector3 &offset) :
		Component(),
		m_colour(new Colour(colour)),
		m_radius(radius),
		m_offset(new Vector3(offset)),
		m_position(new Vector3())
	{
	}

	Light::Light(const Light &source) :
		Component(),
		m_colour(new Colour(*source.m_colour)),
		m_radius(source.m_radius),
		m_offset(new Vector3(*source.m_offset)),
		m_position(new Vector3())
	{
	}

	Light::~Light()
	{
		delete m_colour;
		delete m_offset;
		delete m_position;
	}

	void Light::Update()
	{
		*m_position = *GetGameObject()->GetTransform()->GetPosition() + *m_offset;
	}

	void Light::Load(LoadedValue *value)
	{
		*m_colour = value->GetChild("Colour")->GetString();
		m_radius = value->GetChild("Radius")->Get<float>();
		*m_offset = value->GetChild("Offset");
	}

	void Light::Write(LoadedValue *value)
	{
		value->GetChild("Colour", true)->SetString(m_colour->GetHex());
		value->GetChild("Radius", true)->Set(m_radius);
		m_offset->Write(value->GetChild("Offset", true));
	}

	Light *Light::Set(const Colour &colour, const Vector3 &offset)
	{
		*m_colour = colour;
		*m_offset = offset;
		return this;
	}

	Light *Light::Set(const Colour &colour, const float &radius, const Vector3 &offset)
	{
		*m_colour = colour;
		m_radius = radius;
		*m_offset = offset;
		return this;
	}

	Light *Light::Set(const Light &source)
	{
		*m_colour = *source.m_colour;
		m_radius = source.m_radius;
		*m_offset = *source.m_offset;
		return this;
	}
}
