#pragma once

#include "collider.h"

namespace flounder {
	class sphere : public collider
	{
	private:
		float m_radius;
		vector3 *m_position;
	public:
		/// <summary>
		/// Creates a new unit sphere
		/// </summary>
		sphere();

		/// <summary>
		/// Creates a new sphere
		/// </summary>
		/// <param name="radius"> The spheres radius. </param>
		sphere(const float radius);

		/// <summary>
		/// Creates a new sphere
		/// </summary>
		/// <param name="radius"> The spheres radius. </param>
		/// <param name="position"> The spheres initial position. </param>
		sphere(const float radius, vector3 *position);

		/// <summary>
		/// Creates a new sphere from another sphere source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		sphere(sphere *source);

		/// <summary>
		/// Deconstructor for the sphere.
		/// </summary>
		~sphere();

		collider *update(vector3 *position, vector3 *rotation, const float scale, collider *destination) override;

		vector3 *resolveCollision(collider *other, vector3 *positionDelta, vector3 *destination) override;

		collider *clone() override;

		intersect *intersects(collider *other) override;

		intersect *intersects(ray *other) override;

		bool inFrustum(frustum *frustum) override;

		bool contains(collider *other) override;

		bool contains(vector3 *point) override;

		/// <summary>
		/// Gets the radius of the sphere.
		/// </summary>
		/// <returns> The radius of the sphere. </returns>
		inline float getRadius() { return m_radius; }

		/// <summary>
		/// Sets the radius of the sphere.
		/// </summary>
		/// <param name="radius"> The new sphere radius. </param>
		inline void setRadius(const float radius) { m_radius = radius; };

		/// <summary>
		/// Gets the radius of the position.
		/// </summary>
		/// <returns> The radius of the position. </returns>
		inline vector3 *getPosition() { return m_position; }

		/// <summary>
		/// Sets the position of the sphere.
		/// </summary>
		/// <param name="position"> The new sphere position. </param>
		inline void setPosition(vector3 *position) { m_position->set(position); };
	};
}
