#pragma once

#include "../maths/vector3.h"
#include "../maths/matrix4x4.h"
#include "../physics/frustum.h"
#include "../physics/ray.h"

#include "iplayer.h"

namespace flounder 
{
	/// <summary>
	/// This class is used throughout the engine wherever the camera is involved, so that the engine doesn't rely at all on the camera's implementation.
	/// </summary>
	class icamera 
	{
	public:
		/// <summary>
		/// Creates a new camera.
		/// </summary>
		icamera()
		{
		}

		/// <summary>
		/// Deconstructor for the camera.
		/// </summary>
		virtual ~icamera()
		{
		}

		/// <summary>
		/// Checks inputs and carries out smooth camera movement. Should be called every frame.
		/// </summary>
		/// <param name="player"> The movement and rotation controller to read from. </param>
		virtual void update(const iplayer &player) = 0;

		/// <summary>
		/// Prepares the camera for the reflection render pass.
		/// </summary>
		/// <param name="waterHeight"> The height of the water to be reflected on. </param>
		virtual void reflect(const float &waterHeight) = 0;

		/// <returns> The distance of the near pane of the view frustum. </returns>
		virtual float getNearPlane() const = 0;

		/// <returns> The distance of the view frustum's far plane. </returns>
		virtual float getFarPlane() const = 0;

		/// <returns> The field of view angle for the view frustum. </returns>
		virtual float getFOV() const = 0;

		/// <summary>
		/// Gets the view frustum created by the current camera position and rotation.
		/// </summary>
		/// <returns> The view frustum created by the current camera position and rotation. </returns>
		virtual frustum *getViewFrustum() const = 0;

		/// <summary>
		/// Gets the ray that extends from the cameras position though the screen.
		/// </summary>
		/// <returns> The cameras ray. </returns>
		virtual ray *getViewRay() const = 0;

		/// <summary>
		/// Gets the view matrix created by the current camera position and rotation.
		/// </summary>
		/// <returns> The view matrix created by the current camera position and rotation. </returns>
		virtual matrix4x4 *getViewMatrix() const = 0;

		/// <summary>
		/// Gets the projection matrix used in the current scene render.
		/// </summary>
		/// <returns> The projection matrix used in the current scene render. </returns>
		virtual matrix4x4 *getProjectionMatrix() const = 0;

		/// <summary>
		/// Gets the cameras 3D position in the world.
		/// </summary>
		/// <returns> The cameras 3D position in the world. </returns>
		virtual vector3 *getPosition() const = 0;

		/// <summary>
		/// Gets the cameras 3D rotation in the world, where x=pitch, y=yaw, z=roll.
		/// </summary>
		/// <returns> The cameras 3D rotation in the world, where x=pitch, y=yaw, z=roll. </returns>
		virtual vector3 *getRotation() const = 0;
	};
}