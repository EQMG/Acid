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

UiTransform::UiTransform(const Vector2i &size, const Vector2f &anchor, const Vector2i &offset) :
	m_size(size),
	m_anchor0(anchor),
	m_anchor1(anchor),
	m_offset(offset),
	m_depth(0.0f)
{
}

UiTransform::UiTransform(const Vector2i &size, const Vector2f &anchor0, const Vector2f &anchor1, const Vector2i &offset) :
	m_size(size),
	m_anchor0(anchor0),
	m_anchor1(anchor1),
	m_offset(offset),
	m_depth(0.0f)
{
}

UiTransform::UiTransform(const BitMask<UiMargins> &margins, const Vector2f &anchor0, const Vector2f &anchor1) :
	m_anchor0(anchor0),
	m_anchor1(anchor1),
	m_margins(margins),
	m_depth(0.0f)
{
}

void UiTransform::SetAnchor(const Vector2f &anchor)
{
	m_anchor0 = anchor;
	m_anchor1 = anchor;
}

bool UiTransform::operator==(const UiTransform &other) const
{
	return m_size == other.m_size && m_anchor0 == other.m_anchor0 && m_anchor1 == other.m_anchor1 && m_offset == other.m_offset && m_depth == other.m_depth;
}

bool UiTransform::operator!=(const UiTransform &other) const
{
	return !(*this == other);
}
}
