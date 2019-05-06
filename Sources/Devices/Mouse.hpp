#pragma once

#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"

struct GLFWcursor;

namespace acid
{
enum class MouseButton :
	int32_t
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
	UpperLeft, UpperRight, BottomLeft, BottomRight, Centered
};

enum class CursorStandard
{
	Arrow = 0x00036001,
	IBeam = 0x00036002,
	Crosshair = 0x00036003,
	Hand = 0x00036004,
	ResizeX = 0x00036005,
	ResizeY = 0x00036006
};

/**
 * @brief Module used for managing a virtual mouse.
 */
class ACID_EXPORT Mouse :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Mouse *Get() { return Engine::Get()->GetModule<Mouse>(); }

	Mouse();

	~Mouse();

	void Update() override;

	/**
	 * Gets the current state of a mouse button.
	 * @param mouseButton The mouse button to get the state of.
	 * @return The mouse buttons state.
	 */
	InputAction GetButton(const MouseButton &mouseButton) const;

	/**
	 * Sets the cursor to a image file.
	 * @param filename The new custom mouse file.
	 * @param hotspot The hotspot to display the cursor image at.
	 */
	void SetCursor(const std::string &filename, const CursorHotspot &hotspot);

	/**
	 * Sets the cursor to a system style.
	 * @param standard The standard shape.
	 */
	void SetCursor(const CursorStandard &standard);

	/**
	 * Gets the contents of the clipboard as a string.
	 * @return If the clipboard contents.
	 */
	std::string GetClipboard() const;

	/**
	 * Sets the clipboard to the specified string.
	 * @param string The string to set as the clipboard.
	 */
	void SetClipboard(const std::string &string) const;

	/**
	 * Gets the mouses screen position.
	 * @return The mouses position.
	 */
	const Vector2f &GetPosition() const { return m_mousePosition; }

	/**
	 * Sets the mouse position.
	 * @param position The new position in screen space.
	 */
	void SetPosition(const Vector2f &position);

	/**
	 * Gets the mouse delta.
	 * @return The mouse delta.
	 */
	const Vector2f &GetDelta() const { return m_mouseDelta; }

	/**
	 * Gets the mouse wheel delta.
	 * @return The mouse wheel delta.
	 */
	const Vector2f &GetWheelDelta() const { return m_mouseWheelDelta; }

	/**
	 * Gets if the display is selected.
	 * @return If the display is selected.
	 */
	const bool &IsWindowSelected() const { return m_windowSelected; }

	/**
	 * If the cursor is hidden, the mouse is the display locked if true.
	 * @return If the cursor is hidden.
	 */
	const bool &IsCursorHidden() const { return m_cursorHidden; }

	/**
	 * Sets if the operating systems cursor is hidden whilst in the display.
	 * @param hidden If the system cursor should be hidden when not shown.
	 */
	void SetCursorHidden(const bool &hidden);

	/**
	 * Called when a mouse button changes state.
	 * @return The delegate.
	 */
	Delegate<void(MouseButton, InputAction, BitMask<InputMod>)> &OnButton() { return m_onButton; }

	/**
	 * Called when the mouse moves.
	 * @return The delegate.
	 */
	Delegate<void(Vector2f)> &OnPosition() { return m_onPosition; }

	/**
	 * Called when the mouse enters the window.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnEnter() { return m_onEnter; }

	/**
	 * Called when the scroll wheel changes velocity.
	 * @return The delegate.
	 */
	Delegate<void(Vector2f)> &OnScroll() { return m_onScroll; }

	/**
	 * Called when a group of files/folders is dropped onto the window.
	 * @return The delegate.
	 */
	Delegate<void(std::vector<std::string>)> &OnDrop() { return m_onDrop; }

private:
	static float SmoothScrollWheel(float value, const float &delta);

	friend void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);

	friend void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos);

	friend void CallbackCursorEnter(GLFWwindow *window, int32_t entered);

	friend void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset);

	friend void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths);

	std::optional<std::pair<std::string, CursorHotspot>> m_currentCursor;
	std::optional<CursorStandard> m_currentStandard;
	GLFWcursor *m_cursor;

	Vector2f m_lastMousePosition;
	Vector2f m_mousePosition;
	Vector2f m_mouseDelta;
	Vector2f m_mouseWheelDelta;
	bool m_windowSelected;
	bool m_cursorHidden;

	Delegate<void(MouseButton, InputAction, BitMask<InputMod>)> m_onButton;
	Delegate<void(Vector2f)> m_onPosition;
	Delegate<void(bool)> m_onEnter;
	Delegate<void(Vector2f)> m_onScroll;
	Delegate<void(std::vector<std::string>)> m_onDrop;
};
}
