#pragma once

#include "collider.h"

namespace flounder 
{
	/// <summary>
	/// A axis-aligned bounding box.
	/// </summary>
	class aabb :
		public collider
	{
	private:
		vector3 *m_minExtents;
		vector3 *m_maxExtents;
	public:
		/// <summary>
		/// Creates a new unit aabb.
		/// </summary>
		aabb();

		/// <summary>
		/// Creates a new aabb
		/// </summary>
		/// <param name="minExtents"> The aabbs min extents. </param>
		/// <param name="minExtents"> The aabbs max extents. </param>
		aabb(vector3 *minExtents, vector3 *maxExtents);

		/// <summary>
		/// Creates a new aabb from another aavv source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		aabb(const aabb &source);

		/// <summary>
		/// Deconstructor for the aabb.
		/// </summary>
		~aabb();

		collider *update(const vector3 &position, const vector3 &rotation, const float &scale, collider *destination) override;

		vector3 *resolveCollision(const collider &other, const vector3 &positionDelta, vector3 *destination) override;

		intersect *intersects(const collider &other) override;

		intersect *intersects(const ray &ray) override;

		bool inFrustum(const frustum &frustum) override;

		bool contains(const collider &other) override;

		bool contains(const vector3 &point) override;
	};
}
