#pragma once

#include <optional>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include "Objects/IComponent.hpp"
#include "Maths/Constraint3.hpp"
#include "Maths/Quaternion.hpp"
#include "Maths/Vector3.hpp"
#include "Frustum.hpp"
#include "Ray.hpp"

namespace fl
{
	/// <summary>
	/// A simple class that represents a physics shape.
	/// </summary>
	class FL_EXPORT IShape :
		public IComponent
	{
	public:
		/// <summary>
		/// Creates a new shape.
		/// </summary>
		IShape();

		/// <summary>
		/// Deconstructor for the shape.
		/// </summary>
		virtual ~IShape();

		void Start() override = 0;

		void Update() override = 0;

		virtual void Load(LoadedValue *value) = 0;

		virtual void Write(LoadedValue *destination) = 0;

		virtual std::string GetName() const = 0;

		FL_HIDDEN virtual btCollisionShape *GetCollisionShape() const = 0;

		/// <summary>
		/// Tests whether a ray is intersecting this shape.
		/// </summary>
		/// <param name="ray"> The ray being tested for intersection. </param>
		/// <returns> If the ray intersects, relative intersect location. </returns>
	//	virtual std::optional<Vector3> Raycast(const Ray &ray) = 0;

		/// <summary>
		/// Gets if the shape is partially in the view frustum.
		/// </summary>
		/// <param name="frustum"> The view frustum. </param>
		/// <returns> If the shape is partially in the view frustum. </returns>
		bool InFrustum(const Frustum &frustum);

		FL_HIDDEN static btVector3 Convert(const Vector3 &vector)
		{
			return btVector3(vector.m_x, vector.m_y, vector.m_z);
		}

		FL_HIDDEN static Vector3 Convert(const btVector3 &vector)
		{
			return Vector3(vector.getX(), vector.getY(), vector.getZ());
		}

		FL_HIDDEN static btQuaternion Convert(const Quaternion &quaternion)
		{
			return btQuaternion(quaternion.m_x, quaternion.m_y, quaternion.m_z, quaternion.m_w);
		}

		FL_HIDDEN static Quaternion Convert(const btQuaternion &quaternion)
		{
			return Quaternion(quaternion.getX(), quaternion.getY(), quaternion.getZ(), quaternion.getW());
		}

		FL_HIDDEN static btVector3 Convert(const Constraint3 &constraint)
		{
			return btVector3(constraint.m_x, constraint.m_y, constraint.m_z);
		}
	};
}
