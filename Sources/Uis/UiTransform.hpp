#pragma once

#include "Maths/Vector2.hpp"

namespace acid
{
class ACID_EXPORT UiAnchor
{
public:
	static const Vector2f LeftTop;
	static const Vector2f CentreTop;
	static const Vector2f RightTop;
	static const Vector2f LeftCentre;
	static const Vector2f Centre;
	static const Vector2f RightCentre;
	static const Vector2f LeftBottom;
	static const Vector2f CentreBottom;
	static const Vector2f RightBottom;
};

/**
 * @brief Class that represents a 2D screen space transform.
 */
class ACID_EXPORT UiTransform
{
public:
	explicit UiTransform(const Vector2i &size = Vector2i(100, 100), const Vector2i &position = Vector2i(), 
		const Vector2f &anchor = UiAnchor::LeftTop) noexcept;

	const Vector2f &GetSize() const { return m_size; }

	void SetSize(const Vector2i &size) { m_size = size; }

	const Vector2f &GetPosition() const { return m_position; }

	void SetPosition(const Vector2i &position) { m_position = position; }
	
	const Vector2f &GetAnchor() const { return m_anchor; }

	void SetAnchor(const Vector2f &anchor) { m_anchor = anchor; }

	const float &GetDepth() const { return m_depth; }

	void SetDepth(const float &depth) { m_depth = depth; }

	bool operator==(const UiTransform &other) const;

	bool operator!=(const UiTransform &other) const;

	// TODO: Still needs a way for size inheritance from parent, for background. And margins for expanding menus.

	Vector2f m_size;
	Vector2f m_position;
	Vector2f m_anchor;
	float m_depth;
};
}
