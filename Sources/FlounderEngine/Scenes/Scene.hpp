#pragma once

#include "../Physics/Space/StructureBasic.hpp"
#include "ICamera.hpp"
#include "IManagerUis.hpp"

namespace Flounder
{
	/// <summary>
	/// A object used to represent a scene.
	/// </summary>
	class F_EXPORT Scene
	{
	private:
		ICamera *m_camera;
		IManagerUis *m_managerUis;
		StructureBasic<GameObject *> *m_structure;
	public:
		/// <summary>
		/// Creates a new scene.
		/// </summary>
		/// <param name="camera"> The camera. </param>
		/// <param name="managerUis"> The new uis manager. </param>
		Scene(ICamera *camera, IManagerUis *managerUis);

		/// <summary>
		/// Deconstructor for the scene.
		/// </summary>
		virtual ~Scene();

		virtual void Start() = 0;

		/// <summary>
		/// Run when updating the scene.
		/// </summary>
		virtual void Update() = 0;

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
		/// Gets the uis manager.
		/// </summary>
		/// <returns> The uis manager. </returns>
		IManagerUis *GetUiManager() const { return m_managerUis; };

		/// <summary>
		/// Sets the current uis manager to a new uis manager.
		/// </summary>
		/// <param name="managerUis"> The new uis manager. </param>
		void SetUiManager(IManagerUis *managerUis) { m_managerUis = managerUis; }

		/// <summary>
		/// Gets the GameObjects structure.
		/// </summary>
		/// <returns> The GameObjects structure. </returns>
		StructureBasic<GameObject *> *GetStructure() const { return m_structure; }
	};
}
