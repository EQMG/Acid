#pragma once

#include "../maths/vector3.h"

namespace flounder {
	class iplayer
	{
	public:
		iplayer()
		{
		}

		virtual ~iplayer()
		{
		}

		/// <summary>
		/// Checks inputs and carries out player movement. Should be called every frame.
		/// </summary>
		virtual void update() = 0;

		/// <summary>
		/// Gets the players 3D position in the world.
		/// </summary>
		/// <returns> The players 3D position in the world. </returns>
		virtual vector3 *getPosition() = 0;

		/// <summary>
		/// Gets the players 3D rotation in the world, where x=pitch, y=yaw, z=roll.
		/// </summary>
		/// <returns> The players 3D rotation in the world, where x=pitch, y=yaw, z=roll. </returns>
		virtual vector3 *getRotation() = 0;
	};
}