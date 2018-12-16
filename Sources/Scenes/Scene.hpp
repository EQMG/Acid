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
		friend class Scenes;

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
		/// Gets if the scene is paused.
		/// </summary>
		/// <returns> If the scene is paused. </returns>
		virtual bool IsPaused() const = 0;
	};
}
