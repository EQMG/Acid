#pragma once

namespace fl
{
	/// <summary>
	/// A class that represents bounding intersect data.
	/// </summary>
	class FL_EXPORT Intersect
	{
	private:
		bool m_intersects;
		float m_distance;
	public:
		/// <summary>
		/// Creates a new bounding intersect data.
		/// </summary>
		/// <param name="intersects"> If there is a collision. </param>
		/// <param name="distance"> What distance that collision is at. </param>
		Intersect(const bool &intersects, const float &distance) :
			m_intersects(intersects),
			m_distance(distance)
		{
		}

		/// <summary>
		/// Deconstructor for the intersect.
		/// </summary>
		~Intersect()
		{
		}

		/// <summary>
		/// Gets if there is a intersection.
		/// </summary>
		/// <returns> If there is a intersection. </returns>
		bool IsIntersection() const { return m_intersects; };

		/// <summary>
		/// Gets the distance that intersection is at.
		/// </summary>
		/// <returns> The distance that intersection is at. </returns>
		float Distance() const { return m_distance; };
	};
}
