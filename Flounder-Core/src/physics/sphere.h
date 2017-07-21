#pragma once

#include "collider.h"

namespace flounder
{
	class sphere :
		public collider
	{
	public:
		float m_radius;
		vector3 *m_position;

		/// <summary>
		/// Creates a new unit sphere
		/// </summary>
		sphere();

		/// <summary>
		/// Creates a new sphere
		/// </summary>
		/// <param name="radius"> The spheres radius. </param>
		sphere(const float &radius);

		/// <summary>
		/// Creates a new sphere
		/// </summary>
		/// <param name="radius"> The spheres radius. </param>
		/// <param name="position"> The spheres initial position. </param>
		sphere(const float &radius, vector3 *position);

		/// <summary>
		/// Creates a new sphere from another sphere source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		sphere(const sphere &source);

		/// <summary>
		/// Deconstructor for the sphere.
		/// </summary>
		~sphere();

		collider *update(const vector3 &position, const vector3 &rotation, const float &scale, collider *destination) override;

		vector3 *resolveCollision(const collider &other, const vector3 &positionDelta, vector3 *destination) override;

		intersect *intersects(const collider &other) override;

		intersect *intersects(const ray &ray) override;

		bool inFrustum(const frustum &frustum) override;

		bool contains(const collider &other) override;

		bool contains(const vector3 &point) override;
	};
}
