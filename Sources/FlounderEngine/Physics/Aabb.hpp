#pragma once

#include "ICollider.hpp"

namespace Flounder
{
	/// <summary>
	/// A axis-aligned bounding box.
	/// </summary>
	class Aabb :
		public ICollider
	{
	public:
		Vector3 *m_minExtents;
		Vector3 *m_maxExtents;

		/// <summary>
		/// Creates a new unit aabb.
		/// </summary>
		Aabb();

		/// <summary>
		/// Creates a new aabb
		/// </summary>
		/// <param name="minExtents"> The aabbs min extents. </param>
		/// <param name="minExtents"> The aabbs max extents. </param>
		Aabb(const Vector3 &minExtents, const Vector3 &maxExtents);

		/// <summary>
		/// Creates a new aabb from another aavv source.
		/// </summary>
		/// <param name="source"> The source to create off of. </param>
		Aabb(const Aabb &source);

		/// <summary>
		/// Deconstructor for the aabb.
		/// </summary>
		~Aabb();

		/// <summary>
		/// Loads from another Aabb.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <returns> This. </returns>
		Aabb *Set(const Aabb &source);

		/// <summary>
		/// Creates a new aabb equivalent to this, scaled away from the centre origin.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="scale"> Amount to scale up the aabb. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static Aabb *Scale(const Aabb &source, const Vector3 &scale, Aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, scaled away from the centre origin.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="scaleX"> Amount to scale up the aabb on X. </param>
		/// <param name="scaleY"> Amount to scale up the aabb on Y. </param>
		/// <param name="scaleZ"> Amount to scale up the aabb on Z. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static Aabb *Scale(const Aabb &source, const float &scaleX, const float &scaleY, const float &scaleZ, Aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but scaled away from the origin by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="expand"> Amount to scale up the aabb. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static Aabb *Expand(const Aabb &source, const Vector3 &expand, Aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but scaled away from the origin by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="expandX"> Amount to scale up the aabb on X. </param>
		/// <param name="expandY"> Amount to scale up the aabb on Y. </param>
		/// <param name="expandZ"> Amount to scale up the aabb on Z. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static Aabb *Expand(const Aabb &source, const float &expandX, const float &expandY, const float &expandZ, Aabb *destination);

		/// <summary>
		/// Creates an aabb that bounds both this aabb and another aabb.
		/// </summary>
		/// <param name="left"> The left source aabb. </param>
		/// <param name="right"> The right source aabb. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> An aabb that bounds both this aabb and {@code other}. </returns>
		static Aabb *Combine(const Aabb &left, const Aabb &right, Aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but stretched by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="stretch"> The amount to stretch. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, stretched by the specified amounts. </returns>
		static Aabb *Stretch(const Aabb &source, const Vector3 &stretch, Aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but stretched by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="stretchX"> The amount to stretch on the X. </param>
		/// <param name="stretchY"> The amount to stretch on the Y. </param>
		/// <param name="stretchZ"> The amount to stretch on the Z. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, stretched by the specified amounts. </returns>
		static Aabb *Stretch(const Aabb &source, const float &stretchX, const float &stretchY, const float &stretchZ, Aabb *destination);

		ICollider *Update(const Transform &transform, ICollider *destination) override;

		Vector3 *ResolveCollision(const ICollider &other, const Vector3 &positionDelta, Vector3 *destination) override;

		Intersect Intersects(const ICollider &other) override;

		Intersect Intersects(const Ray &ray) override;

		bool InFrustum(const Frustum &frustum) override;

		bool Contains(const ICollider &other) override;

		bool Contains(const Vector3 &point) override;

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

		Vector3 *GetMinExtents() const { return m_minExtents; }

		void SetMinExtents(const Vector3 &minExtents) const { m_minExtents->Set(minExtents); }

		Vector3 *GetMaxExtents() const { return m_maxExtents; }

		void SetMaxExtents(const Vector3 &maxExtents) const { m_maxExtents->Set(maxExtents); }
	};
}
