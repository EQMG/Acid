#pragma once

#include <map>
#include "Maths/Vector2.hpp"

namespace Flounder
{
	/// <summary>
	/// A rectangle made of a position and dimension.
	/// </summary>
	class F_EXPORT UiBound
	{
	private:
		static std::map<std::string, Vector2> PIVOT_MAP;
	public:
		Vector2 *m_position;
		Vector2 *m_reference;
		bool m_aspectPosition;
		bool m_aspectSize;
		Vector2 *m_dimensions;

		/// <summary>
		/// Constructor for rectangle.
		/// </summary>
		/// <param name="position"> The object screen position. </param>
		/// <param name="reference"> The reference pivot vertex (TopLeft, TopCentre, TopRight, CentreLeft, Centre, CentreRight, BottomLeft, BottomCentre, BottomRight). </param>
		/// <param name="aspectPosition"> The if the x position will change with display width. </param>
		/// <param name="aspectSize"> The if the width will scale with display width. </param>
		/// <param name="dimensions"> The object dimensions. </param>
		UiBound(const Vector2 &position, const std::string &reference = "TopLeft", const bool &aspectPosition = true, const bool &aspectSize = true, const Vector2 &dimensions = Vector2(1.0f, 1.0f));

		/// <summary>
		/// Creates a new rectangle.
		/// </summary>
		/// <param name="source"> Creates this rectangle out of a existing one. </param>
		UiBound(const UiBound &source);

		/// <summary>
		/// Deconstructor for rectangle.
		/// </summary>
		~UiBound();

		/// <summary>
		/// Loads from another Rectangle.
		/// </summary>
		/// <param name="source"> The source rectangle. </param>
		/// <returns> This. </returns>
		UiBound *Set(const UiBound &source);

		static Vector2 FindPivot(const std::string &key);
	};
}
