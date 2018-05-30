#pragma once

#include "ICollider.hpp"

namespace fl
{
	/// <summary>
	/// Represents a sphere in a 3d space.
	/// </summary>
	class FL_EXPORT ColliderSphere :
		public ICollider
	{
	public:
		float m_radius;
		Vector3 m_position;

		/// <summary>
		/// Creates a new sphere
		/// </summary>
		/// <param name="radius"> The spheres radius. </param>
		/// <param name="position"> The spheres initial position. </param>
		ColliderSphere(const float &radius = 0.0f, const Vector3 &position = Vector3::ZERO);

		/// <summary>
		/// Creates a new sphere from another sphere source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		ColliderSphere(const ColliderSphere &source);

		/// <summary>
		/// Deconstructor for the sphere.
		/// </summary>
		~ColliderSphere();

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		ICollider *UpdateCollider(const Transform &transform, ICollider *destination) override;

		Vector3 ResolveCollision(const ICollider &other, const Vector3 &positionStart, const Vector3 &positionDelta) override;

		Intersect Intersects(const ICollider &other) override;

		Intersect Intersects(const Ray &ray) override;

		bool InFrustum(const Frustum &frustum) override;

		bool Contains(const ICollider &other) override;

		bool Contains(const Vector3 &point) override;

		std::string GetName() const override { return "ColliderSphere"; };

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { m_position = position; }

		ColliderSphere &operator=(const ColliderSphere &other);

		bool operator==(const ColliderSphere &other) const;

		bool operator!=(const ColliderSphere &other) const;
	};
}
