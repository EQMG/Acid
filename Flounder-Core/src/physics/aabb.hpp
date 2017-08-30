#pragma once

#include "icollider.hpp"

namespace flounder
{
	/// <summary>
	/// A axis-aligned bounding box.
	/// </summary>
	class aabb :
		public icollider
	{
	public:
		vector3 *m_minExtents;
		vector3 *m_maxExtents;

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

		/// <summary>
		/// Creates a new aabb equivalent to this, scaled away from the centre origin.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="scale"> Amount to scale up the aabb. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static aabb *scale(const aabb &source, const vector3 &scale, aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, scaled away from the centre origin.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="scaleX"> Amount to scale up the aabb on X. </param>
		/// <param name="scaleY"> Amount to scale up the aabb on Y. </param>
		/// <param name="scaleZ"> Amount to scale up the aabb on Z. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static aabb *scale(const aabb &source, const float &scaleX, const float &scaleY, const float &scaleZ, aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but scaled away from the origin by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="expand"> Amount to scale up the aabb. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static aabb *expand(const aabb &source, const vector3 &expand, aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but scaled away from the origin by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="expandX"> Amount to scale up the aabb on X. </param>
		/// <param name="expandY"> Amount to scale up the aabb on Y. </param>
		/// <param name="expandZ"> Amount to scale up the aabb on Z. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, scaled by the specified amounts. </returns>
		static aabb *expand(const aabb &source, const float &expandX, const float &expandY, const float &expandZ, aabb *destination);

		/// <summary>
		/// Creates an aabb that bounds both this aabb and another aabb.
		/// </summary>
		/// <param name="left"> The left source aabb. </param>
		/// <param name="right"> The right source aabb. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> An aabb that bounds both this aabb and {@code other}. </returns>
		static aabb *combine(const aabb &left, const aabb &right, aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but stretched by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="stretch"> The amount to stretch. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, stretched by the specified amounts. </returns>
		static aabb *stretch(const aabb &source, const vector3 &stretch, aabb *destination);

		/// <summary>
		/// Creates a new aabb equivalent to this, but stretched by a certain amount.
		/// </summary>
		/// <param name="source"> The source aabb. </param>
		/// <param name="stretchX"> The amount to stretch on the X. </param>
		/// <param name="stretchY"> The amount to stretch on the Y. </param>
		/// <param name="stretchZ"> The amount to stretch on the Z. </param>
		/// <param name="destination"> The destination aabb or null if a new aabb is to be created. </param>
		/// <returns> A new aabb, stretched by the specified amounts. </returns>
		static aabb *stretch(const aabb &source, const float &stretchX, const float &stretchY, const float &stretchZ, aabb *destination);

		icollider *update(const vector3 &position, const vector3 &rotation, const float &scale, icollider *destination) override;

		vector3 *resolveCollision(const icollider &other, const vector3 &positionDelta, vector3 *destination) override;

		intersect *intersects(const icollider &other) override;

		intersect *intersects(const ray &ray) override;

		bool inFrustum(const frustum &frustum) override;

		bool contains(const icollider &other) override;

		bool contains(const vector3 &point) override;

		/// <summary>
		/// Calculates the centre of this aabb on the X axis.
		/// </summary>
		/// <returns> The centre location of this aabb on the X axis. </returns>
		float getCentreX();

		/// <summary>
		/// Calculates the centre of this aabb on the Y axis.
		/// </summary>
		/// <returns> The centre location of this aabb on the Y axis. </returns>
		float getCentreY();

		/// <summary>
		/// Calculates the centre of this aabb on the Z axis.
		/// </summary>
		/// <returns> The centre location of this aabb on the Z axis. </returns>
		float getCentreZ();

		/// <summary>
		/// Gets the width of this aabb.
		/// </summary>
		/// <returns> The width of this aabb. </returns>
		float getWidth();

		/// <summary>
		/// Gets the height of this aabb.
		/// </summary>
		/// <returns> The height of this aabb. </returns>
		float getHeight();

		/// <summary>
		/// Gets the depth of this aabb.
		/// </summary>
		/// <returns> The depth of this aabb. </returns>
		float getDepth();

		inline vector3 *getMinExtents() const { return m_minExtents; }

		inline void setMinExtents(const vector3 &minExtents) const { m_minExtents->set(minExtents); }

		inline vector3 *getMaxExtents() const { return m_maxExtents; }

		inline void setMaxExtents(const vector3 &maxExtents) const { m_maxExtents->set(maxExtents); }
	};
}
