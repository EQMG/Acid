#pragma once

#include "Maths/Vector2.hpp"
#include "Helpers/EnumClass.hpp"

namespace acid
{
class ACID_EXPORT UiReference
{
public:
	static const Vector2f TopLeft;
	static const Vector2f TopCentre;
	static const Vector2f TopRight;
	static const Vector2f CentreLeft;
	static const Vector2f Centre;
	static const Vector2f CentreRight;
	static const Vector2f BottomLeft;
	static const Vector2f BottomCentre;
	static const Vector2f BottomRight;
};

enum class UiAspect :
	uint32_t
{
	None = 0, 
	Position = 1, 
	Size = 2,
	Scale = 4
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
	 * @param size The object size.
	 */
	explicit UiBound(const Vector2f &position = Vector2f(0.0f, 0.0f), const Vector2f &reference = UiReference::TopLeft,
		const BitMask<UiAspect> &aspect = UiAspect::Position | UiAspect::Size, const Vector2f &size = Vector2f(1.0f, 1.0f)) noexcept;

	/**
	 * Gets the bounds position in the current screen space.
	 * @param aspectRatio The screens acpect ratio.
	 * @return The position in screen space.
	 */
	Vector2f GetScreenPosition(const float &aspectRatio) const;

	/**
	 * Gets the bounds size in the current screen space.
	 * @param aspectRatio The screens acpect ratio.
	 * @return The size in screen space.
	 */
	Vector2f GetScreenSize(const float &aspectRatio) const;

	const Vector2f &GetPosition() const { return m_position; }

	void SetPosition(const Vector2f &position) { m_position = position; }

	const Vector2f &GetReference() const { return m_reference; }

	void SetReference(const Vector2f &reference) { m_reference = reference; }

	const BitMask<UiAspect> &GetAspect() const { return m_aspect; }

	void SetAspect(const BitMask<UiAspect> &aspect) { m_aspect = aspect; }

	const Vector2f &GetSize() const { return m_size; }

	void SetSize(const Vector2f &size) { m_size = size; }

	bool operator==(const UiBound &other) const;

	bool operator!=(const UiBound &other) const;

	static const UiBound Screen;
	static const UiBound Maximum;
	static const UiBound Left;
	static const UiBound Centre;
	static const UiBound Right;

	Vector2f m_position;
	Vector2f m_reference;
	BitMask<UiAspect> m_aspect;
	Vector2f m_size;
};
}
