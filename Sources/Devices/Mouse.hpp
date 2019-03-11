#pragma once

#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

namespace acid
{
	enum class MouseButton : int32_t
	{
		_1 = 0,
		_2 = 1,
		_3 = 2,
		_4 = 3,
		_5 = 4,
		_6 = 5,
		_7 = 6,
		_8 = 7,
		Left = _1,
		Right = _2,
		Middle = _3,
		First = _1,
		Last = _8
	};

	enum class CursorHotspot
	{
		UpperLeft,
		UpperRight,
		BottomLeft,
		BottomRight,
		Centered
	};

	/// <summary>
	/// A module used for the creation, updating and destruction of the mouse.
	/// </summary>
	class ACID_EXPORT Mouse :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Mouse *Get() { return Engine::Get()->GetModuleManager().Get<Mouse>(); }

		Mouse();

		void Update() override;

		/// <summary>
		/// Gets whether or not a particular mouse button is currently pressed.
		/// </summary>
		/// <param name="mouseButton"> The mouse button to test. </param>
		/// <returns> If the mouse button is currently pressed. </returns>
		InputAction GetButton(const MouseButton &mouseButton) const;

		/// <summary>
		/// Sets the custom mouse file.
		/// </summary>
		/// <param name="filename"> The new custom mouse file. </param>
		/// <param name="hotspot"> The hotspot to display the cursor image at. </param>
		void SetCursor(const std::string &filename, const CursorHotspot &hotspot);

		/// <summary>
		/// Gets the contents of the clipboard as a string.
		/// </summary>
		/// <returns> If the clipboard contents. </returns>
		std::string GetClipboard() const;

		/// <summary>
		/// Sets the clipboard to the specified string.
		/// </summary>
		/// <param name="string"> The string to set as the clipboard. </param>
		void SetClipboard(const std::string &string) const;

		/// <summary>
		/// Gets the mouses screen x position.
		/// </summary>
		/// <returns> The mouses x position. </returns>
		const float &GetPositionX() const { return m_mousePositionX; }

		/// <summary>
		/// Gets the mouses screen y position.
		/// </summary>
		/// <returns> The mouses y position. </returns>
		const float &GetPositionY() const { return m_mousePositionY; }

		/// <summary>
		/// Gets the mouses screen position.
		/// </summary>
		/// <returns> The mouses position. </returns>
		Vector2 GetPosition() { return Vector2(GetPositionX(), GetPositionY()); }

		/// <summary>
		/// Sets the mouse position.
		/// </summary>
		/// <param name="cursorX"> The x position in screen space. </param>
		/// <param name="cursorY"> The y position in screen space. </param>
		void SetPosition(const float &cursorX, const float &cursorY);

		/// <summary>
		/// Sets the mouse position.
		/// </summary>
		/// <param name="position"> The position in screen space. </param>
		void SetPosition(const Vector2 &position);

		/// <summary>
		/// Gets the mouses delta.
		/// </summary>
		/// <returns> The mouses delta. </returns>
		Vector2 GetDelta() const { return Vector2(GetDeltaX(), GetDeltaY()); }

		/// <summary>
		/// Gets the mouses delta x.
		/// </summary>
		/// <returns> The mouses delta x. </returns>
		const float &GetDeltaX() const { return m_mouseDeltaX; }

		/// <summary>
		/// Gets the mouses delta y.
		/// </summary>
		/// <returns> The mouses delta y. </returns>
		const float &GetDeltaY() const { return m_mouseDeltaY; }

		/// <summary>
		/// Gets the mouses wheel delta.
		/// </summary>
		/// <returns> The mouses wheel delta. </returns>
		const float &GetDeltaWheel() const { return m_mouseDeltaWheel; }

		/// <summary>
		/// Gets if the display is selected.
		/// </summary>
		/// <returns> If the display is selected. </returns>
		const bool &IsWindowSelected() const { return m_windowSelected; }

		/// <summary>
		/// If the cursor is hidden, the mouse is the display locked if true.
		/// </summary>
		/// <returns> If the cursor is hidden. </returns>
		const bool &IsCursorHidden() const { return m_cursorHidden; }

		/// <summary>
		/// Sets if the operating systems cursor is hidden whilst in the display.
		/// </summary>
		/// <param name="hidden"> If the system cursor should be hidden when not shown. </param>
		void SetCursorHidden(const bool &hidden);

		Delegate<void(MouseButton, InputAction, BitMask<InputMod>)> &GetOnButton() { return m_onButton; }

		Delegate<void(float, float)> &GetOnPosition() { return m_onPosition; }

		Delegate<void(bool)> &GetOnEnter() { return m_onEnter; }

		Delegate<void(float, float)> &GetOnScroll() { return m_onScroll; }

		Delegate<void(std::vector<std::string>)> &GetOnDrop() { return m_onDrop; }
	private:
		float m_lastMousePositionX;
		float m_lastMousePositionY;
		float m_mousePositionX;
		float m_mousePositionY;
		float m_mouseDeltaX;
		float m_mouseDeltaY;
		float m_mouseDeltaWheel;
		bool m_windowSelected;
		bool m_cursorHidden;

		Delegate<void(MouseButton, InputAction, BitMask<InputMod>)> m_onButton;
		Delegate<void(float, float)> m_onPosition;
		Delegate<void(bool)> m_onEnter;
		Delegate<void(float, float)> m_onScroll;
		Delegate<void(std::vector<std::string>)> m_onDrop;

		friend void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);

		friend void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos);

		friend void CallbackCursorEnter(GLFWwindow *window, int32_t entered);

		friend void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset);

		friend void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths);
	};
}
