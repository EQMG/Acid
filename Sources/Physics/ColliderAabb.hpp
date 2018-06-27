#pragma once

#include "ICollider.hpp"

namespace fl
{
	/// <summary>
	/// A axis-aligned bounding box.
	/// </summary>
	class FL_EXPORT ColliderAabb :
		public ICollider
	{
	public:
		Vector3 m_minExtents;
		Vector3 m_maxExtents;

		/// <summary>
		/// Creates a new aabb
		/// </summary>
		/// <param name="minExtents"> The aabbs min extents. </param>
		/// <param name="minExtents"> The aabbs max extents. </param>
		ColliderAabb(const Vector3 &minExtents = Vector3::ZERO, const Vector3 &maxExtents = Vector3::ZERO);

		/// <summary>
		/// Creates a new aabb from another aavv source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		ColliderAabb(const ColliderAabb &source);

		/// <summary>
		/// Deconstructor for the aabb.
		/// </summary>
		~ColliderAabb();

		/// <summary>
		/// Scales this aabb by a scalar vector.
		/// </summary>
		/// <param name="scale"> Amount to scale up the aabb. </param>
		/// <returns> The resultant aabb. </returns>
		ColliderAabb Scale(const Vector3 &scale);

		/// <summary>
		/// Expands this aabb away from the origin by a expansion vector.
		/// </summary>
		/// <param name="expandX"> Amount to scale up the aabb. </param>
		/// <returns> The resultant aabb. </returns>
		ColliderAabb Expand(const Vector3 &expand);

		/// <summary>
		/// Combines two aabbs.
		/// </summary>
		/// <param name="left"> The left source aabb. </param>
		/// <param name="right"> The right source aabb. </param>
		/// <returns> The resultant aabb. </returns>
		ColliderAabb Combine(const ColliderAabb &other);

		/// <summary>
		/// Stretched this aabb by a vector.
		/// </summary>
		/// <param name="stretch"> The amount to stretch. </param>
		/// <returns> The resultant aabb. </returns>
		ColliderAabb Stretch(const Vector3 &stretch);

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;

		ICollider *UpdateCollider(const Transform &transform, ICollider *destination) override;

		Intersect Intersects(const ICollider &other) override;

		Intersect Intersects(const Ray &ray) override;

		bool InFrustum(const Frustum &frustum) override;

		bool Contains(const ICollider &other) override;

		bool Contains(const Vector3 &point) override;

		std::string GetName() const override { return "ColliderAabb"; };

		/// <summary>
		/// Calculates the centre of this aabb on the X axis.
		/// </summary>
		/// <returns> The centre location of this aabb on the X axis. </returns>
		float GetCentreX() const;

		/// <summary>
		/// Calculates the centre of this aabb on the Y axis.
		/// </summary>
		/// <returns> The centre location of this aabb on the Y axis. </returns>
		float GetCentreY() const;

		/// <summary>
		/// Calculates the centre of this aabb on the Z axis.
		/// </summary>
		/// <returns> The centre location of this aabb on the Z axis. </returns>
		float GetCentreZ() const;

		/// <summary>
		/// Gets the width of this aabb.
		/// </summary>
		/// <returns> The width of this aabb. </returns>
		float GetWidth() const;

		/// <summary>
		/// Gets the height of this aabb.
		/// </summary>
		/// <returns> The height of this aabb. </returns>
		float GetHeight() const;

		/// <summary>
		/// Gets the depth of this aabb.
		/// </summary>
		/// <returns> The depth of this aabb. </returns>
		float GetDepth() const;

		Vector3 GetMinExtents() const { return m_minExtents; }

		void SetMinExtents(const Vector3 &minExtents) { m_minExtents = minExtents; }

		Vector3 GetMaxExtents() const { return m_maxExtents; }

		void SetMaxExtents(const Vector3 &maxExtents) { m_maxExtents = maxExtents; }

		ColliderAabb &operator=(const ColliderAabb &other);

		bool operator==(const ColliderAabb &other) const;

		bool operator!=(const ColliderAabb &other) const;
	};
}
