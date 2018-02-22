#pragma once

#include "Collider.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a sphere in a 3d space.
	/// </summary>
	class F_EXPORT Sphere :
		public Collider
	{
	public:
		float m_radius;
		Vector3 *m_position;

		/// <summary>
		/// Creates a new unit sphere
		/// </summary>
		Sphere();

		/// <summary>
		/// Creates a new sphere
		/// </summary>
		/// <param name="radius"> The spheres radius. </param>
		/// <param name="position"> The spheres initial position. </param>
		Sphere(const float &radius, const Vector3 &position = Vector3());

		/// <summary>
		/// Creates a new sphere from another sphere source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		Sphere(const Sphere &source);

		/// <summary>
		/// Deconstructor for the sphere.
		/// </summary>
		~Sphere();

		Collider *Update(const Transform &transform, Collider *destination) override;

		Vector3 *ResolveCollision(const Collider &other, const Vector3 &positionDelta, Vector3 *destination) override;

		Intersect Intersects(const Collider &other) override;

		Intersect Intersects(const Ray &ray) override;

		bool InFrustum(const Frustum &frustum) override;

		bool Contains(const Collider &other) override;

		bool Contains(const Vector3 &point) override;

		std::string GetName() const override { return "ColliderSphere"; };

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 *GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) const { m_position->Set(position); }
	};
}
