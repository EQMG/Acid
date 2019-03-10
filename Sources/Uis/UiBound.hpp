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

	enum class UiAspect : uint32_t
	{
		None = 0,
		Position = 1,
		Dimensions = 2,
		Scale = 4
	};

	ENABLE_BITMASK_OPERATORS(UiAspect)

	/// <summary>
	/// A rectangle made of a position and dimension.
	/// </summary>
	class ACID_EXPORT UiBound
	{
	public:
		/// <summary>
		/// Constructor for rectangle.
		/// </summary>
		/// <param name="position"> The object screen position. </param>
		/// <param name="reference"> The reference vertex of the parent bounds, where position is. </param>
		/// <param name="aspect"> The aspect that will be used for bounding in the parent reference. </param>
		/// <param name="dimensions"> The object dimensions. </param>
		explicit UiBound(const Vector2 &position = Vector2(0.0f, 0.0f), const Vector2 &reference = UiReference::TopLeft,
			const Bitmask<UiAspect> &aspect = UiAspect::Position | UiAspect::Dimensions, const Vector2 &dimensions = Vector2(1.0f, 1.0f));

		/// <summary>
		/// Gets the bounds position in the current screen space.
		/// </summary>
		/// <returns> The position in screen space. </returns>
		Vector2 GetScreenPosition(const float &aspectRatio) const;

		/// <summary>
		/// Gets the bounds dimensions in the current screen space.
		/// </summary>
		/// <returns> The dimensions in screen space. </returns>
		Vector2 GetScreenDimensions(const float &aspectRatio) const;

		const Vector2 &GetPosition() const { return m_position; }

		void SetPosition(const Vector2 &position) { m_position = position; }

		const Vector2 &GetReference() const { return m_reference; }

		void SetReference(const Vector2 &reference) { m_reference = reference; }

		const Bitmask<UiAspect> &GetAspect() const { return m_aspect; }

		void SetAspect(const Bitmask<UiAspect> &aspect) { m_aspect = aspect; }

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
		Bitmask<UiAspect> m_aspect;
		Vector2 m_dimensions;
	};
}
