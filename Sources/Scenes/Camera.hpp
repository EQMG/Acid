#pragma once

#include "Engine/Exports.hpp"
#include "Scenes/Component.hpp"
#include "Maths/Matrix4.hpp"
#include "Maths/Vector3.hpp"
#include "Physics/Frustum.hpp"
#include "Physics/Ray.hpp"

namespace acid
{
	/// <summary>
	/// This component represents a scene camera.
	/// </summary>
	class ACID_EXPORT Camera :
		public Component
	{
	public:
		~Camera() = default;

		/// <summary>
		/// Gets the distance of the near pane of the view frustum.
		/// </summary>
		/// <returns> The distance of the near pane of the view frustum. </returns>
		virtual float GetNearPlane() const
		{
			return 0.1f;
		}

		/// <summary>
		/// Gets the distance of the view frustum's far plane.
		/// </summary>
		/// <returns> The distance of the view frustum's far plane. </returns>
		virtual float GetFarPlane() const
		{
			return 1000.0f;
		}

		/// <summary>
		/// Gets the field of view angle for the view frustum.
		/// </summary>
		/// <returns> The field of view angle for the view frustum. </returns>
		virtual float GetFov() const
		{
			return 180.0f;
		}

		/// <summary>
		/// Gets the view frustum created by the current camera position and rotation.
		/// </summary>
		/// <returns> The view frustum created by the current camera position and rotation. </returns>
		virtual Frustum GetViewFrustum() const
		{
			return Frustum::ZERO;
		}

		/// <summary>
		/// Gets the ray that extends from the cameras position though the screen.
		/// </summary>
		/// <returns> The cameras view ray. </returns>
		virtual Ray GetViewRay() const
		{
			return Ray::ZERO;
		}

		/// <summary>
		/// Gets the view matrix created by the current camera position and rotation.
		/// </summary>
		/// <returns> The view matrix created by the current camera position and rotation. </returns>
		virtual Matrix4 GetViewMatrix() const
		{
			return Matrix4::IDENTITY;
		}

		/// <summary>
		/// Gets the projection matrix used in the current scene render.
		/// </summary>
		/// <returns> The projection matrix used in the current scene render. </returns>
		virtual Matrix4 GetProjectionMatrix() const
		{
			return Matrix4::IDENTITY;
		}

		/// <summary>
		/// Gets the cameras 3D position in the world.
		/// </summary>
		/// <returns> The cameras 3D position in the world. </returns>
		virtual Vector3 GetPosition() const
		{
			return Vector3::ZERO;
		}

		/// <summary>
		/// Gets the cameras 3D rotation in the world, where x=pitch, y=yaw, z=roll.
		/// </summary>
		/// <returns> The cameras 3D rotation in the world, where x=pitch, y=yaw, z=roll. </returns>
		virtual Vector3 GetRotation() const
		{
			return Vector3::ZERO;
		}

		/// <summary>
		/// Gets the cameras 3D velocity in the world.
		/// </summary>
		/// <returns> The cameras 3D velocity in the world. </returns>
		virtual Vector3 GetVelocity() const
		{
			return Vector3::ZERO;
		}
	};
}
