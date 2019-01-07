#pragma once

#include "Maths/Vector2.hpp"

namespace acid
{
	/// <summary>
	/// A rectangle made of a position and dimension.
	/// </summary>
	class ACID_EXPORT UiBound
	{
	public:
		Vector2 m_position;
		Vector2 m_reference;
		bool m_aspectPosition;
		bool m_aspectSize;
		Vector2 m_dimensions;

		static const Vector2 TOP_LEFT;
		static const Vector2 TOP_CENTRE;
		static const Vector2 TOP_RIGHT;

		static const Vector2 CENTRE_LEFT;
		static const Vector2 CENTRE;
		static const Vector2 CENTRE_RIGHT;

		static const Vector2 BOTTOM_LEFT;
		static const Vector2 BOTTOM_CENTRE;
		static const Vector2 BOTTOM_RIGHT;

		/// <summary>
		/// Constructor for rectangle.
		/// </summary>
		/// <param name="position"> The object screen position. </param>
		/// <param name="reference"> The reference pivot vertex (<seealso cref="#TOP_LEFT"/>, <seealso cref="#CENTRE_LEFT"/>, <seealso cref="#BOTTOM_LEFT"/>, ETC). </param>
		/// <param name="aspectPosition"> The if the x position will change with display width. </param>
		/// <param name="aspectSize"> The if the width will scale with display width. </param>
		/// <param name="dimensions"> The object dimensions. </param>
		explicit UiBound(const Vector2 &position, const Vector2 &reference = TOP_LEFT, const bool &aspectPosition = true, const bool &aspectSize = true, const Vector2 &dimensions = Vector2(1.0f, 1.0f));

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

		const bool &IsAspectPosition() const { return m_aspectPosition; }

		void SetAspectPosition(const bool &aspectPosition) { m_aspectPosition = aspectPosition; }

		const bool &IsAspectSize() const { return m_aspectSize; }

		void SetAspectSize(const bool &aspectSize) { m_aspectSize = aspectSize; }

		const Vector2 &GetDimensions() const { return m_dimensions; }

		void SetDimensions(const Vector2 &dimensions) { m_dimensions = dimensions; }

		bool operator==(const UiBound &other) const;

		bool operator!=(const UiBound &other) const;
	};
}
