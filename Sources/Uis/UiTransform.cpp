#include "UiTransform.hpp"

namespace acid
{
const Vector2f UiAnchor::TopLeft = Vector2f(0.0f, 0.0f);
const Vector2f UiAnchor::TopCentre = Vector2f(0.5f, 0.0f);
const Vector2f UiAnchor::TopRight = Vector2f(1.0f, 0.0f);
const Vector2f UiAnchor::CentreLeft = Vector2f(0.0f, 0.5f);
const Vector2f UiAnchor::Centre = Vector2f(0.5f, 0.5f);
const Vector2f UiAnchor::CentreRight = Vector2f(1.0f, 0.5f);
const Vector2f UiAnchor::BottomLeft = Vector2f(0.0f, 1.0f);
const Vector2f UiAnchor::BottomCentre = Vector2f(0.5f, 1.0f);
const Vector2f UiAnchor::BottomRight = Vector2f(1.0f, 1.0f);

const UiTransform UiTransform::Screen = UiTransform(Vector2f(0.0f, 0.0f), UiAnchor::TopLeft, UiAspect::Position);
const UiTransform UiTransform::Maximum = UiTransform(Vector2f(0.5f, 0.5f), UiAnchor::Centre, UiAspect::Position | UiAspect::Scale);
const UiTransform UiTransform::Left = UiTransform(Vector2f(0.0f, 0.5f), UiAnchor::CentreLeft);
const UiTransform UiTransform::Centre = UiTransform(Vector2f(0.5f, 0.5f), UiAnchor::Centre);
const UiTransform UiTransform::Right = UiTransform(Vector2f(1.0f, 0.5f), UiAnchor::CentreRight);

UiTransform::UiTransform(const Vector2f &position, const Vector2f &anchor, const BitMask<UiAspect> &aspect, const Vector2f &scale) noexcept :
	m_position(position),
	m_anchor(anchor),
	m_aspect(aspect),
	m_scale(scale)
{
}

Vector2f UiTransform::GetScreenPosition(const float &aspectRatio) const
{
	Vector2f screenPosition = m_position;

	if (!(m_aspect & UiAspect::Position))
	{
		screenPosition.m_x /= aspectRatio;
	}

	return screenPosition;
}

Vector2f UiTransform::GetScreenSize(const float &aspectRatio) const
{
	Vector2f screenSize = m_scale;

	if (m_aspect & UiAspect::Size)
	{
		screenSize.m_x /= aspectRatio;
	}

	return screenSize;
}

bool UiTransform::operator==(const UiTransform &other) const
{
	return m_position == other.m_position && m_anchor == other.m_anchor && m_aspect == other.m_aspect && m_scale == other.m_scale;
}

bool UiTransform::operator!=(const UiTransform &other) const
{
	return !(*this == other);
}
}
