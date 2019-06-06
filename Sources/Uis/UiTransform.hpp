#pragma once

#include "Maths/Vector2.hpp"
#include "Helpers/EnumClass.hpp"

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

enum class UiMargins
{
	None = 0,
	Left = 1,
	Right = 2,
	Top = 4,
	Bottom = 8,
	LeftTop = Left | Top,
	RightBottom = Right | Bottom,
	All = Left | Right | Top | Bottom
};

ENABLE_BITMASK_OPERATORS(UiMargins)

/**
 * @brief Class that represents a 2D screen space transform.
 */
class ACID_EXPORT UiTransform
{
public:
	/**
	 * Creates a new UiTransform.
	 */
	UiTransform() = default;

	/**
	 * Creates a new UiTransform.
	 * @param size The size of the rectangle in pixels.
	 * @param anchor The reference anchor where the rectangle will be placed relative in the parent.
	 * @param position The pixel offset from the anchor position.
	 */
	UiTransform(const Vector2i &size, const Vector2f &anchor = UiAnchor::LeftTop, const Vector2i &position = {});

	/**
	 * Creates a new UiTransform.
	 * @param size The size of the rectangle in pixels.
	 * @param anchor0 The reference anchor where the rectangle will be placed relative in the parent.
	 * @param anchor1 The anchor inside of this rectangle.
	 * @param position The pixel offset from the anchor position.
	 */
	UiTransform(const Vector2i &size, const Vector2f &anchor0, const Vector2f &anchor1, const Vector2i &position);

	/**
	 * Creates a new UiTransform.
	 * @param margins What margins will be in percentage from the parent.
	 * @param anchor0 The offset from the left top vertex in pixels or percentage of the parents size.
	 * @param anchor1 The offset from the right bottom vertex in pixels or percentage of the parents size.
	 */
	UiTransform(const BitMask<UiMargins> &margins, const Vector2f &anchor0 = {}, const Vector2f &anchor1 = {});

	const Vector2f &GetSize() const { return m_size; }

	void SetSize(const Vector2i &size) { m_size = size; }

	const Vector2f &GetAnchor0() const { return m_anchor0; }

	void SetAnchor0(const Vector2f &anchor0) { m_anchor0 = anchor0; }

	const Vector2f &GetAnchor1() const { return m_anchor1; }

	void SetAnchor1(const Vector2f &anchor1) { m_anchor1 = anchor1; }

	void SetAnchor(const Vector2f &anchor);

	const Vector2f &GetPosition() const { return m_position; }

	void SetPosition(const Vector2i &position) { m_position = position; }

	const std::optional<BitMask<UiMargins>> &GetMargins() const { return m_margins; }

	void SetMargins(const std::optional<BitMask<UiMargins>> &margins) { m_margins = margins; }
	
	const float &GetDepth() const { return m_depth; }

	void SetDepth(const float &depth) { m_depth = depth; }

	bool operator==(const UiTransform &other) const;

	bool operator!=(const UiTransform &other) const;

	Vector2f m_size;
	Vector2f m_anchor0, m_anchor1;
	Vector2f m_position;
	std::optional<BitMask<UiMargins>> m_margins;
	float m_depth{};
};
}
