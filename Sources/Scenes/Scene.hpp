#pragma once

#include <memory>
#include "Uis/UiSelector.hpp"
#include "Camera.hpp"
#include "ScenePhysics.hpp"
#include "SceneStructure.hpp"

namespace acid
{
	/// <summary>
	/// A object used to represent a scene.
	/// </summary>
	class ACID_EXPORT Scene
	{
	private:
		std::unique_ptr<Camera> m_camera;
		std::unique_ptr<SelectorJoystick> m_selectorJoystick;
		std::unique_ptr<ScenePhysics> m_physics;
		std::unique_ptr<SceneStructure> m_structure;
		bool m_started;
	public:
		/// <summary>
		/// Creates a new scene.
		/// </summary>
		/// <param name="camera"> The scenes camera. </param>
		/// <param name="selectorJoystick"> The joystick controlled UI selector. </param>
		Scene(Camera *camera, SelectorJoystick *selectorJoystick) :
			m_camera(camera),
			m_selectorJoystick(selectorJoystick),
			m_physics(std::make_unique<ScenePhysics>()),
			m_structure(std::make_unique<SceneStructure>()),
			m_started(false)
		{
		}

		virtual void Start() = 0;

		/// <summary>
		/// Run when updating the scene.
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// Gets the current camera object.
		/// </summary>
		/// <returns> The current camera. </returns>
		Camera *GetCamera() const { return m_camera.get(); }

		/// <summary>
		/// Sets the current camera to a new camera.
		/// </summary>
		/// <param name="camera"> The new camera. </param>
		void SetCamera(Camera *camera) { m_camera.reset(camera); }

		/// <summary>
		/// Gets the joystick controlled UI selector.
		/// </summary>
		/// <returns> The joystick selector. </returns>
		SelectorJoystick *GetSelectorJoystick() const { return m_selectorJoystick.get(); };

		/// <summary>
		/// Sets the joystick controlled UI selector.
		/// </summary>
		/// <param name="selectorJoystick"> The new joystick selector. </param>
		void SetSelectorJoystick(SelectorJoystick *selectorJoystick) { m_selectorJoystick.reset(selectorJoystick); }

		/// <summary>
		/// Gets the scene physics system.
		/// </summary>
		/// <returns> The scenes physics system. </returns>
		ScenePhysics *GetPhysics() const { return m_physics.get(); }

		/// <summary>
		/// Gets the scene object structure.
		/// </summary>
		/// <returns> The scene object structure. </returns>
		SceneStructure *GetStructure() { return m_structure.get(); }

		/// <summary>
		/// Creates a new entity that starts in this structure.
		/// </summary>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		/// <returns> The newly created entity. </returns>
		Entity *CreateEntity(const Transform &transform) { return m_structure->CreateEntity(transform); }

		/// <summary>
		/// Creates a new entity from a prefab that starts in this structure.
		/// </summary>
		/// <param name="filename"> The file to load the component data from. </param>
		/// <param name="transform"> The objects initial world position, rotation, and scale. </param>
		/// <returns> The newly created entity. </returns>
		Entity *CreateEntity(const std::string &filename, const Transform &transform) { return m_structure->CreateEntity(filename, transform); }

		/// <summary>
		/// Gets if this scene has started.
		/// </summary>
		/// <returns> If the scene has started. </returns>
		bool IsStarted() const { return m_started; }

		/// <summary>
		/// Sets if this scene has started.
		/// </summary>
		/// <param name="started"> If the scene has started. </param>
		void SetStarted(const bool &started) { m_started = started; }

		/// <summary>
		/// Gets if the scene is paused.
		/// </summary>
		/// <returns> If the scene is paused. </returns>
		virtual bool IsPaused() const = 0;
	};
}
