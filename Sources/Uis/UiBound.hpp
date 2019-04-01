#pragma once

#include "Maths/Vector2.hpp"
#include "Helpers/EnumClass.hpp"

namespace acid
{
class ACID_EXPORT UiReference
{
public:
	static const Vector2 TopLeft;
	static const Vector2 TopCentre;
	static const Vector2 TopRight;
	static const Vector2 CentreLeft;
	static const Vector2 Centre;
	static const Vector2 CentreRight;
	static const Vector2 BottomLeft;
	static const Vector2 BottomCentre;
	static const Vector2 BottomRight;
};

enum class UiAspect :
	uint32_t
{
	None = 0, Position = 1, Dimensions = 2, Scale = 4
};

ENABLE_BITMASK_OPERATORS(UiAspect)

/**
 * @brief Class that represents a rectangle made of a position and dimension.
 */
class ACID_EXPORT UiBound
{
public:
	/**
	 * Creates a new ui bound.
	 * @param position The object screen position.
	 * @param reference The reference vertex of the parent bounds, where position is.
	 * @param aspect The aspect that will be used for bounding in the parent reference.
	 * @param dimensions The object dimensions.
	 */
	explicit UiBound(const Vector2 &position = Vector2(0.0f, 0.0f), const Vector2 &reference = UiReference::TopLeft,
		const BitMask<UiAspect> &aspect = UiAspect::Position | UiAspect::Dimensions, const Vector2 &dimensions = Vector2(1.0f, 1.0f)) noexcept;

	/**
	 * Gets the bounds position in the current screen space.
	 * @param aspectRatio The screens acpect ratio.
	 * @return The position in screen space.
	 */
	Vector2 GetScreenPosition(const float &aspectRatio) const;

	/**
	 * Gets the bounds dimensions in the current screen space.
	 * @param aspectRatio The screens acpect ratio.
	 * @return The dimensions in screen space.
	 */
	Vector2 GetScreenDimensions(const float &aspectRatio) const;

	const Vector2 &GetPosition() const { return m_position; }

	void SetPosition(const Vector2 &position) { m_position = position; }

	const Vector2 &GetReference() const { return m_reference; }

	void SetReference(const Vector2 &reference) { m_reference = reference; }

	const BitMask<UiAspect> &GetAspect() const { return m_aspect; }

	void SetAspect(const BitMask<UiAspect> &aspect) { m_aspect = aspect; }

	const Vector2 &GetDimensions() const { return m_dimensions; }

	void SetDimensions(const Vector2 &dimensions) { m_dimensions = dimensions; }

	bool operator==(const UiBound &other) const;

	bool operator!=(const UiBound &other) const;

	static const UiBound Screen;
	static const UiBound Maximum;
	static const UiBound Left;
	static const UiBound Centre;
	static const UiBound Right;

	Vector2 m_position;
	Vector2 m_reference;
	BitMask<UiAspect> m_aspect;
	Vector2 m_dimensions;
};
}
