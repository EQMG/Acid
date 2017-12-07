#pragma once

#include "../Engine/Engine.hpp"
#include "ICamera.hpp"
#include "IPlayer.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing cameras in 2D and 3D worlds.
	/// </summary>
	class Camera :
		public IModule
	{
	private:
		ICamera *m_camera;
		IPlayer *m_player;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Camera *Get()
		{
			return static_cast<Camera*>(Engine::Get()->GetModule("camera"));
		}

		/// <summary>
		/// Creates a new camera module.
		/// </summary>
		Camera();

		/// <summary>
		/// Deconstructor for the camera module.
		/// </summary>
		~Camera();

		void Update() override;

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		ICamera *GetCamera() const { return m_camera; }

		/// <summary>
		/// Sets the current camera to a new camera.
		/// </summary>
		/// <param name="camera"> The new camera. </param>
		void SetCamera(ICamera *camera) { m_camera = camera; }

		/// <summary>
		/// Gets the current player object.
		/// </summary>
		/// <returns> The current player. </returns>
		IPlayer *GetPlayer() const { return m_player; }

		/// <summary>
		/// Sets the current player to a new player.
		/// </summary>
		/// <param name="player"> The new player. </param>
		void SetPlayer(IPlayer *player) { m_player = player; }
	};
}
