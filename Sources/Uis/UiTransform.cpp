#include "UiTransform.hpp"

namespace acid {
const Vector2f UiAnchor::LeftTop(0.0f, 0.0f);
const Vector2f UiAnchor::CentreTop(0.5f, 0.0f);
const Vector2f UiAnchor::RightTop(1.0f, 0.0f);
const Vector2f UiAnchor::LeftCentre(0.0f, 0.5f);
const Vector2f UiAnchor::Centre(0.5f, 0.5f);
const Vector2f UiAnchor::RightCentre(1.0f, 0.5f);
const Vector2f UiAnchor::LeftBottom(0.0f, 1.0f);
const Vector2f UiAnchor::CentreBottom(0.5f, 1.0f);
const Vector2f UiAnchor::RightBottom(1.0f, 1.0f);

UiTransform::UiTransform(const Vector2f &size, const Vector2f &anchor, const Vector2f &position) :
	m_size(size),
	m_anchor0(anchor),
	m_anchor1(anchor),
	m_position(position) {
}

UiTransform::UiTransform(const Vector2f &size, const Vector2f &anchor0, const Vector2f &anchor1, const Vector2f &position) :
	m_size(size),
	m_anchor0(anchor0),
	m_anchor1(anchor1),
	m_position(position) {
}

UiTransform::UiTransform(const BitMask<UiMargins> &margins, const Vector2f &anchor0, const Vector2f &anchor1) :
	m_anchor0(anchor0),
	m_anchor1(anchor1),
	m_margins(margins) {
}

bool UiTransform::operator==(const UiTransform &other) const {
	return m_size == other.m_size && m_anchor0 == other.m_anchor0 && m_anchor1 == other.m_anchor1 && m_position == other.m_position && m_depth == other.m_depth;
}

bool UiTransform::operator!=(const UiTransform &other) const {
	return !operator==(other);
}

void UiTransform::SetAnchor(const Vector2f &anchor) {
	m_anchor0 = anchor;
	m_anchor1 = anchor;
}
}
