#pragma once

#include <optional>
#include "Maths/Quaternion.hpp"
#include "Maths/Vector3.hpp"
#include "Objects/IComponent.hpp"
#include "Frustum.hpp"
#include "Ray.hpp"

class btCollisionShape;

class btVector3;

class btQuaternion;

namespace acid
{
	/// <summary>
	/// A simple class that represents a physics shape.
	/// </summary>
	class ACID_EXPORT Collider :
		public IComponent
	{
	public:
		Collider();

		virtual ~Collider();

		void Start() override = 0;

		void Update() override = 0;

		virtual void Decode(const Metadata &metadata) override = 0;

		virtual void Encode(Metadata &metadata) const override = 0;

		virtual btCollisionShape* GetCollisionShape() const = 0;

		/// <summary>
		/// Gets if the shape is partially in the view frustum.
		/// </summary>
		/// <param name="frustum"> The view frustum. </param>
		/// <returns> If the shape is partially in the view frustum. </returns>
		bool InFrustum(const Frustum &frustum);

		/// <summary>
		/// Tests whether a ray is intersecting this shape.
		/// </summary>
		/// <param name="ray"> The ray being tested for intersection. </param>
		/// <returns> If the ray intersects, relative intersect location. </returns>
	//	virtual std::optional<Vector3> Raycast(const Ray &ray) = 0;

		static btVector3 Convert(const Vector3 &vector);

		static Vector3 Convert(const btVector3 &vector);

		static btQuaternion Convert(const Quaternion &quaternion);

		static Quaternion Convert(const btQuaternion &quaternion);
	};
}
