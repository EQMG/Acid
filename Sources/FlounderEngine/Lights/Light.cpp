#include "Light.hpp"

#include "../Helpers/FormatString.hpp"

namespace Flounder
{
	Light::Light(const Colour &colour, const float &radius, const Vector3 &offset) :
		Component(),
		m_colour(new Colour(colour)),
		m_radius(radius),
		m_offset(new Vector3(offset)),
		m_position(new Vector3())
	{
		LinkString(0, LINK_GET_STR(Colour::GetHex(*GetColour())), LINK_SET(std::string, SetColour(Colour(v))));
		Link<float>(1, LINK_GET(GetRadius()), LINK_SET(float, SetRadius(v)));
		Link<float>(2, LINK_GET(GetOffset()->m_x), LINK_SET(float, GetOffset()->m_x = v));
		Link<float>(3, LINK_GET(GetOffset()->m_y), LINK_SET(float, GetOffset()->m_y = v));
		Link<float>(4, LINK_GET(GetOffset()->m_z), LINK_SET(float, GetOffset()->m_z = v));
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
		m_position->Set(*m_gameObject->GetTransform()->m_position + *m_offset);
	}

	Light *Light::Set(const Colour &colour, const Vector3 &offset)
	{
		m_colour->Set(colour);
		m_offset->Set(offset);
		return this;
	}

	Light *Light::Set(const Colour &colour, const float &radius, const Vector3 &offset)
	{
		m_colour->Set(colour);
		m_radius = radius;
		m_offset->Set(offset);
		return this;
	}

	Light *Light::Set(const Light &source)
	{
		m_colour->Set(*source.m_colour);
		m_radius = source.m_radius;
		m_offset->Set(*source.m_offset);
		return this;
	}
}
