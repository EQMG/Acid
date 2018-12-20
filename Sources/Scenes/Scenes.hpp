#pragma once

#include "Engine/Engine.hpp"
#include "Models/ModelRegister.hpp"
#include "Scene.hpp"
#include "ComponentRegister.hpp"
#include "SceneStructure.hpp"

namespace acid
{
	/// <summary>
	/// A module used for managing game scenes on engine updates.
	/// </summary>
	class ACID_EXPORT Scenes :
		public Module
	{
	private:
		std::unique_ptr<Scene> m_scene;

		ComponentRegister m_componentRegister;
		ModelRegister m_modelRegister;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Scenes *Get() { return Engine::Get()->GetModuleManager().Get<Scenes>(); }

		/// <summary>
		/// Creates a new Scenes module.
		/// </summary>
		Scenes();

		~Scenes() = default;

		void Update() override;

		/// <summary>
		/// Gets the current scene.
		/// </summary>
		/// <returns> The current scene. </returns>
		Scene *GetScene() const { return m_scene.get(); }

		/// <summary>
		/// Sets the current scene to a new scene.
		/// </summary>
		/// <param name="scene"> The new scene. </param>
		void SetScene(Scene *scene) { m_scene.reset(scene); }

		/// <summary>
		/// Gets the component register used by the engine. The register can be used to register/deregister component types.
		/// </summary>
		/// <returns> The component register. </returns>
		ComponentRegister &GetComponentRegister() { return m_componentRegister; }

		/// <summary>
		/// Gets the model register used by the engine. The register can be used to register/deregister model types.
		/// </summary>
		/// <returns> The model register. </returns>
		ModelRegister &GetModelRegister() { return m_modelRegister; }

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		Camera *GetCamera() const { return m_scene != nullptr ? m_scene->GetCamera() : nullptr; }

		/// <summary>
		/// Gets the joystick controlled UI selector.
		/// </summary>
		/// <returns> The joystick selector. </returns>
		SelectorJoystick *GetSelectorJoystick() const { return m_scene != nullptr ? m_scene->GetSelectorJoystick() : nullptr; }

		/// <summary>
		/// Gets the scene physics system.
		/// </summary>
		/// <returns> The scenes physics system. </returns>
		ScenePhysics *GetPhysics() const { return m_scene != nullptr ? m_scene->GetPhysics() : nullptr; }

		/// <summary>
		/// Gets the scene object structure.
		/// </summary>
		/// <returns> The scene object structure. </returns>
		SceneStructure *GetStructure() const { return m_scene != nullptr ? m_scene->GetStructure() : nullptr; }

		/// <summary>
		/// Gets if the scene is paused.
		/// </summary>
		/// <returns> If the scene is paused. </returns>
		bool IsPaused() const { return m_scene != nullptr ? m_scene->IsPaused() : false; }
	};
}
