#pragma once

#include <memory>
#include "ICamera.hpp"
#include "SceneStructure.hpp"
#include "Uis/UiSelector.hpp"

namespace acid
{
	/// <summary>
	/// A object used to represent a scene.
	/// </summary>
	class ACID_EXPORT IScene
	{
	private:
		ICamera *m_camera;
		SceneStructure *m_structure;
		bool m_started;
	public:
		/// <summary>
		/// Creates a new scene.
		/// </summary>
		/// <param name="camera"> The scenes camera. </param>
		IScene(ICamera *camera) :
			m_camera(camera),
			m_structure(new SceneStructure()),
			m_started(false)
		{
		}

		/// <summary>
		/// Deconstructor for the scene.
		/// </summary>
		virtual ~IScene()
		{
			delete m_camera;
			delete m_structure;
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
		ICamera *GetCamera() const { return m_camera; }

		/// <summary>
		/// Sets the current camera to a new camera.
		/// </summary>
		/// <param name="camera"> The new camera. </param>
		void SetCamera(ICamera *camera)
		{
			delete m_camera; // TODO: Cleanup.
			m_camera = camera;
		}

		/// <summary>
		/// Gets the GameObjects structure.
		/// </summary>
		/// <returns> The GameObjects structure. </returns>
		SceneStructure *GetStructure() const { return m_structure; }

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
		virtual bool IsGamePaused() = 0;

		/// <summary>
		/// The primary colour to be used in UI elements.
		/// </summary>
		/// <returns> The primary colour. </returns>
		virtual Colour *GetUiColour() const = 0;

		/// <summary>
		/// The UI selector for a joystick.
		/// </summary>
		/// <returns> The joystick selector. </returns>
		virtual SelectorJoystick *GetSelectorJoystick() const = 0;
	};
}
