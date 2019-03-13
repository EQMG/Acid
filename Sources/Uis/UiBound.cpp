#include "UiBound.hpp"

namespace acid
{
const Vector2 UiReference::TopLeft = Vector2(0.0f, 0.0f);
const Vector2 UiReference::TopCentre = Vector2(0.5f, 0.0f);
const Vector2 UiReference::TopRight = Vector2(1.0f, 0.0f);
const Vector2 UiReference::CentreLeft = Vector2(0.0f, 0.5f);
const Vector2 UiReference::Centre = Vector2(0.5f, 0.5f);
const Vector2 UiReference::CentreRight = Vector2(1.0f, 0.5f);
const Vector2 UiReference::BottomLeft = Vector2(0.0f, 1.0f);
const Vector2 UiReference::BottomCentre = Vector2(0.5f, 1.0f);
const Vector2 UiReference::BottomRight = Vector2(1.0f, 1.0f);

const UiBound UiBound::Screen = UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft, UiAspect::Position);
const UiBound UiBound::Maximum = UiBound(Vector2(0.5f, 0.5f), UiReference::Centre, UiAspect::Position | UiAspect::Scale);
const UiBound UiBound::Left = UiBound(Vector2(0.0f, 0.5f), UiReference::CentreLeft);
const UiBound UiBound::Centre = UiBound(Vector2(0.5f, 0.5f), UiReference::Centre);
const UiBound UiBound::Right = UiBound(Vector2(1.0f, 0.5f), UiReference::CentreRight);

UiBound::UiBound(const Vector2& position, const Vector2& reference, const BitMask<UiAspect>& aspect, const Vector2& dimensions) : m_position(position), m_reference(reference), m_aspect(aspect), m_dimensions(dimensions) {}

Vector2 UiBound::GetScreenPosition(const float& aspectRatio) const
{
	Vector2 screenPosition = m_position;

	if(!(m_aspect & UiAspect::Position))
		{
			screenPosition.m_x /= aspectRatio;
		}

	return screenPosition;
}

Vector2 UiBound::GetScreenDimensions(const float& aspectRatio) const
{
	Vector2 screenDimensions = m_dimensions;

	if(m_aspect & UiAspect::Dimensions)
		{
			screenDimensions.m_x /= aspectRatio;
		}

	return screenDimensions;
}

bool UiBound::operator==(const UiBound& other) const
{
	return m_position == other.m_position && m_reference == other.m_reference && m_aspect == other.m_aspect && m_dimensions == other.m_dimensions;
}

bool UiBound::operator!=(const UiBound& other) const
{
	return !(*this == other);
}
}
