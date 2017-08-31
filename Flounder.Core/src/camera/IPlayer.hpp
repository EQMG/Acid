#pragma once

#include "../maths/Vector3.hpp"

namespace Flounder
{
	/// <summary>
	/// This interface is used to move and add extra rotation to a camera.
	/// </summary>
	class IPlayer
	{
	public:
		/// <summary>
		/// Creates a new player.
		/// </summary>
		IPlayer()
		{
		}

		/// <summary>
		/// Deconstructor for the player.
		/// </summary>
		virtual ~IPlayer()
		{
		}

		/// <summary>
		/// Checks inputs and carries out player movement. Should be called every frame.
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// Gets the players 3D position in the world.
		/// </summary>
		/// <returns> The players 3D position in the world. </returns>
		virtual Vector3 *GetPosition() const = 0;

		/// <summary>
		/// Gets the players 3D rotation in the world, where x=pitch, y=yaw, z=roll.
		/// </summary>
		/// <returns> The players 3D rotation in the world, where x=pitch, y=yaw, z=roll. </returns>
		virtual Vector3 *GetRotation() const = 0;
	};
}
