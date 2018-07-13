#pragma once

#include "IBounding.hpp"

namespace fl
{
	/// <summary>
	/// Represents a sphere in a 3d space.
	/// </summary>
	class FL_EXPORT BoundingSphere :
		public IBounding
	{
	public:
		float m_radius;
		Vector3 m_position;

		/// <summary>
		/// Creates a new sphere
		/// </summary>
		/// <param name="radius"> The spheres radius. </param>
		/// <param name="position"> The spheres initial position. </param>
		BoundingSphere(const float &radius = 0.0f, const Vector3 &position = Vector3::ZERO);

		/// <summary>
		/// Creates a new sphere from another sphere source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		BoundingSphere(const BoundingSphere &source);

		/// <summary>
		/// Deconstructor for the sphere.
		/// </summary>
		~BoundingSphere();

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		IBounding *UpdateCollider(const Transform &transform, IBounding *destination) override;

		Intersect Intersects(const IBounding &other) override;

		Intersect Intersects(const Ray &ray) override;

		bool InFrustum(const Frustum &frustum) override;

		bool Contains(const IBounding &other) override;

		bool Contains(const Vector3 &point) override;

		std::string GetName() const override { return "BoundingSphere"; };

		float GetRadius() const { return m_radius; }

		void SetRadius(const float &radius) { m_radius = radius; }

		Vector3 GetPosition() const { return m_position; }

		void SetPosition(const Vector3 &position) { m_position = position; }

		BoundingSphere &operator=(const BoundingSphere &other);

		bool operator==(const BoundingSphere &other) const;

		bool operator!=(const BoundingSphere &other) const;
	};
}
