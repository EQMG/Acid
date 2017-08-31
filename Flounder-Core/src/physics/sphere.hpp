#pragma once

#include "icollider.hpp"

namespace Flounder
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
		/// <param name="position"> The spheres initial position. </param>
		sphere(const float &radius, const vector3 &position = vector3());

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

		float getRadius() const { return m_radius; }

		void setRadius(const float &radius) { m_radius = radius; }

		vector3 *getPosition() const { return m_position; }

		void setPosition(const vector3 &position) const { m_position->set(position); }
	};
}
