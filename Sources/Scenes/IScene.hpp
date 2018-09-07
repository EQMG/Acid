#pragma once

#include <memory>
#include "Uis/UiSelector.hpp"
#include "ICamera.hpp"
#include "ScenePhysics.hpp"
#include "SceneStructure.hpp"

namespace acid
{
	/// <summary>
	/// A object used to represent a scene.
	/// </summary>
	class ACID_EXPORT IScene
	{
	private:
		std::unique_ptr<ICamera> m_camera;
		std::unique_ptr<ScenePhysics> m_physics;
		std::unique_ptr<SceneStructure> m_structure;
		bool m_started;
	public:
		/// <summary>
		/// Creates a new scene.
		/// </summary>
		/// <param name="camera"> The scenes camera. </param>
		IScene(ICamera *camera) :
			m_camera(camera),
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
		ICamera *GetCamera() const { return m_camera.get(); }

		/// <summary>
		/// Sets the current camera to a new camera.
		/// </summary>
		/// <param name="camera"> The new camera. </param>
		void SetCamera(ICamera *camera) { m_camera.reset(camera); }

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
		/// Gets if the main menu is open.
		/// </summary>
		/// <returns> If the main menu is open. </returns>
		virtual bool IsGamePaused() const = 0;

		/// <summary>
		/// The primary colour to be used in UI elements.
		/// </summary>
		/// <returns> The primary colour. </returns>
		virtual Colour GetUiColour() const = 0;

		/// <summary>
		/// The UI selector for a joystick.
		/// </summary>
		/// <returns> The joystick selector. </returns>
		virtual SelectorJoystick GetSelectorJoystick() const = 0;
	};
}
