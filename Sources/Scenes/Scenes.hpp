#pragma once

#include "Engine/Engine.hpp"
#include "Objects/ComponentRegister.hpp"
#include "SceneStructure.hpp"
#include "Scene.hpp"

namespace fl
{
	/// <summary>
	/// A module used for managing game scenes on engine updates.
	/// </summary>
	class FL_EXPORT Scenes :
		public IModule
	{
	private:
		ComponentRegister *m_componentRegister;
		Scene *m_scene;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Scenes *Get()
		{
			return reinterpret_cast<Scenes *>(Engine::Get()->GetModule("scenes"));
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

		/// <summary>
		/// Registers a component with the register.
		/// </summary>
		/// <param name="update"> The components update type. </param>
		/// <param name="name"> The components name. </param>
		/// <param name="T"> The components type. </param>
		template<typename T>
		void RegisterComponent(const std::string &name) { m_componentRegister->RegisterComponent<T>(name); }

		/// <summary>
		/// Deregisters a component.
		/// </summary>
		/// <param name="name"> The components name. </param>
		void DeregisterComponent(const std::string &name) { m_componentRegister->DeregisterComponent(name); }

		/// <summary>
		/// Creates a new component from the register.
		/// </summary>
		/// <param name="name"> The component name to create. </param>
		/// <returns> The new component. </returns>
		std::shared_ptr<Component> CreateComponent(const std::string &name) { return m_componentRegister->CreateComponent(name); }

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
		SceneStructure *GetStructure() const { return m_scene->GetStructure(); }
	};
}
