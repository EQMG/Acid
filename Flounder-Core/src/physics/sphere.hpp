#pragma once

#include "icollider.hpp"

namespace flounder
{
	/// <summary>
	/// Represents a sphere in a 3d space.
	/// </summary>
	class sphere :
		public icollider
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

		icollider *update(const vector3 &position, const vector3 &rotation, const float &scale, icollider *destination) override;

		vector3 *resolveCollision(const icollider &other, const vector3 &positionDelta, vector3 *destination) override;

		intersect *intersects(const icollider &other) override;

		intersect *intersects(const ray &ray) override;

		bool inFrustum(const frustum &frustum) override;

		bool contains(const icollider &other) override;

		bool contains(const vector3 &point) override;
	};
}
