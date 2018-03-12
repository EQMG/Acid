#pragma once

#include "../Engine/Engine.hpp"
#include "../Objects/GameObject.hpp"
#include "../Physics/Space/StructureBasic.hpp"
#include "Scene.hpp"

namespace Flounder
{
	/// <summary>
	/// A module used for managing game scenes on engine updates.
	/// </summary>
	class F_EXPORT Scenes :
		public IModule
	{
	private:
		Scene *m_scene;
	public:
		static Scenes *S_INSTANCE;

		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Scenes *Get()
		{
			return S_INSTANCE; // reinterpret_cast<Scenes *>(Engine::Get()->GetModule("scenes"));
		}

		/// <summary>
		/// Creates a new Scenes module.
		/// </summary>
		Scenes();

		/// <summary>
		/// Deconstructor for the Scenes module.
		/// </summary>
		~Scenes();

		void Update() override;

		Scene *GetScene() const { return m_scene; }

		void SetScene(Scene *scene);

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		ICamera *GetCamera() const { return m_scene->GetCamera(); }

		/// <summary>
		/// Gets the uis manager.
		/// </summary>
		/// <returns> The uis manager. </returns>
		IManagerUis *GetUiManager() const { return m_scene->GetUiManager(); };

		/// <summary>
		/// Gets if the scene is paused.
		/// </summary>
		/// <returns> If the scene is paused. </returns>
		bool IsGamePaused() { return m_scene->GetUiManager()->IsGamePaused(); }

		/// <summary>
		/// Gets the GameObjects structure.
		/// </summary>
		/// <returns> The GameObjects structure. </returns>
		StructureBasic<GameObject *> *GetStructure() const { return m_scene->GetStructure(); }
	};
}
