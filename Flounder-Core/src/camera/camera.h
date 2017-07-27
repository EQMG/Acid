#pragma once

#include "../framework/framework.h"

#include "icamera.h"
#include "iplayer.h"

namespace flounder
{
	/// <summary>
	/// A module used for managing cameras in 2D and 3D worlds.
	/// </summary>
	class camera :
		public imodule
	{
	private:
		icamera *m_camera;
		iplayer *m_player;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static camera *get()
		{
			return static_cast<camera*>(framework::get()->getInstance("camera"));
		}

		/// <summary>
		/// Creates a new camera module.
		/// </summary>
		camera();

		/// <summary>
		/// Deconstructor for the camera module.
		/// </summary>
		~camera();

		void update() override;

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		inline icamera *getCamera() { return m_camera; }

		/// <summary>
		/// Sets the current camera to a new camera.
		/// </summary>
		/// <param name="camera"> The new camera. </param>
		inline void setCamera(icamera *camera) { m_camera = camera; }

		/// <summary>
		/// Gets the current player object.
		/// </summary>
		/// <returns> The current player. </returns>
		inline iplayer *getPlayer() { return m_player; }

		/// <summary>
		/// Sets the current player to a new player.
		/// </summary>
		/// <param name="player"> The new player. </param>
		inline void setPlayer(iplayer *player) { m_player = player; }
	};
}
