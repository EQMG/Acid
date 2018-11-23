#pragma once

#include <optional>
#include <memory>
#include "Maths/Quaternion.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Transform.hpp"
#include "Objects/IComponent.hpp"
#include "Frustum.hpp"
#include "Ray.hpp"

class btCollisionShape;

class btVector3;

class btQuaternion;

class btTransform;

namespace acid
{
	/// <summary>
	/// A simple class that represents a physics shape.
	/// </summary>
	class ACID_EXPORT Collider :
		public IComponent
	{
	protected:
		Transform m_localTransform;
	public:
		/// <summary>
		/// Creates a new collider shape.
		/// </summary>
		/// <param name="localTransform"> The parent offset of the body. </param>
		Collider(const Transform &localTransform = Transform::ZERO);

		/// <summary>
		/// Gets the collision shape defined in this collider.
		/// </summary>
		/// <returns> The collision shape. </returns>
		virtual btCollisionShape* GetCollisionShape() const = 0;

		/// <summary>
		/// Tests whether a ray is intersecting this shape.
		/// </summary>
		/// <param name="ray"> The ray being tested for intersection. </param>
		/// <returns> If the ray intersects, relative intersect location. </returns>
	//	virtual std::optional<Vector3> Raycast(const Ray &ray) = 0;

		Transform &GetLocalTransform() { return m_localTransform; }

		void SetLocalTransform(const Transform &localTransform);

		static btVector3 Convert(const Vector3 &vector);

		static Vector3 Convert(const btVector3 &vector);

		static btQuaternion Convert(const Quaternion &quaternion);

		static Quaternion Convert(const btQuaternion &quaternion);

		static btTransform Convert(const Transform &transform);

		static Transform Convert(const btTransform &transform, const Vector3 &scaling = Vector3::ONE);
	};
}
