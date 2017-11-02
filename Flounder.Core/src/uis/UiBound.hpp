#pragma once

#include "../maths/Vector2.hpp"
#include <map>

namespace Flounder
{
	/// <summary>
	/// The aspect bools used in the object, if the position x / dimension width will be scaled with the screen aspect ratio.
	/// </summary>
	enum Aspect
	{
		AspectPosition = 1 << 0,
		AspectSize = 1 << 1
	};

	/// <summary>
	/// A rectangle made of a position and dimension.
	/// </summary>
	class UiBound
	{
	private:
		static std::map<std::string, Vector2> g_pivotMap;
	public:
		Vector2 *m_position;
		Vector2 *m_dimensions;
		int m_flagsAspect;
		Vector2 *m_reference;

		/// <summary>
		/// Constructor for rectangle.
		/// </summary>
		/// <param name="position"> The object screen position. </param>
		/// <param name="dimensions"> The object dimensions. </param>
		/// <param name="flagsAspect"> The flags that will define the aspects. </param>
		/// <param name="flagsEdges"> The flags that will define the pivot vertex. </param>
		UiBound(const Vector2 &position, const Vector2 &dimensions, const int &flagsAspect = AspectPosition | AspectSize, const Vector2 &reference = Vector2(0.0f, 1.0f));

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
