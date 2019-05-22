#include "UiTransform.hpp"

namespace acid
{
const Vector2f UiAnchor::LeftTop = Vector2f(0.0f, 0.0f);
const Vector2f UiAnchor::CentreTop = Vector2f(0.5f, 0.0f);
const Vector2f UiAnchor::RightTop = Vector2f(1.0f, 0.0f);
const Vector2f UiAnchor::LeftCentre = Vector2f(0.0f, 0.5f);
const Vector2f UiAnchor::Centre = Vector2f(0.5f, 0.5f);
const Vector2f UiAnchor::RightCentre = Vector2f(1.0f, 0.5f);
const Vector2f UiAnchor::LeftBottom = Vector2f(0.0f, 1.0f);
const Vector2f UiAnchor::CentreBottom = Vector2f(0.5f, 1.0f);
const Vector2f UiAnchor::RightBottom = Vector2f(1.0f, 1.0f);

UiTransform::UiTransform(const Vector2i &size, const Vector2i &position, const Vector2f &anchor) :
	m_size(size),
	m_position(position),
	m_anchor(anchor),
	m_depth(0.0f)
{
}

bool UiTransform::operator==(const UiTransform &other) const
{
	return m_size == other.m_size && m_position == other.m_position && m_anchor == other.m_anchor && m_depth == other.m_depth;
}

bool UiTransform::operator!=(const UiTransform &other) const
{
	return !(*this == other);
}
}
