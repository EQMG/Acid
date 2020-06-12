#pragma once

#include "Devices/Window.hpp"
#include "Engine/Engine.hpp"

struct GLFWcursor;

namespace acid {
enum class MouseButton : uint8_t {
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

enum class CursorHotspot : uint8_t {
	UpperLeft,
	UpperRight,
	BottomLeft,
	BottomRight,
	Centered
};

enum class CursorStandard : uint32_t {
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
class ACID_EXPORT Mouse : public Module::Registrar<Mouse, Module::Stage::Pre> {
public:
	Mouse();
	~Mouse();

	void Update() override;

	/**
	 * Gets the current state of a mouse button.
	 * @param mouseButton The mouse button to get the state of.
	 * @return The mouse buttons state.
	 */
	InputAction GetButton(MouseButton mouseButton) const;

	/**
	 * Sets the cursor to a image file.
	 * @param filename The new custom mouse file.
	 * @param hotspot The hotspot to display the cursor image at.
	 */
	void SetCursor(const std::filesystem::path &filename, CursorHotspot hotspot);

	/**
	 * Sets the cursor to a system style.
	 * @param standard The standard shape.
	 */
	void SetCursor(CursorStandard standard);

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
	 * Gets the mouses position.
	 * @return The mouses position.
	 */
	const Vector2d &GetPosition() const { return position; }

	/**
	 * Sets the mouse position.
	 * @param position The new mouse position.
	 */
	void SetPosition(const Vector2d &position);

	/**
	 * Gets the mouse position delta.
	 * @return The mouse position delta.
	 */
	const Vector2d &GetPositionDelta() const { return positionDelta; }

	/**
	 * Gets the mouses virtual scroll position.
	 * @return The mouses virtual scroll position.
	 */
	const Vector2d &GetScroll() const { return scroll; }

	/**
	 * Sets the mouse virtual scroll position.
	 * @param scroll The new mouse virtual scroll position.
	 */
	void SetScroll(const Vector2d &scroll);

	/**
	 * Gets the mouse scroll delta.
	 * @return The mouse scroll delta.
	 */
	const Vector2d &GetScrollDelta() const { return scrollDelta; }

	/**
	 * Gets if the display is selected.
	 * @return If the display is selected.
	 */
	bool IsWindowSelected() const { return windowSelected; }

	/**
	 * If the cursor is hidden, the mouse is the display locked if true.
	 * @return If the cursor is hidden.
	 */
	bool IsCursorHidden() const { return cursorHidden; }

	/**
	 * Sets if the operating systems cursor is hidden whilst in the display.
	 * @param hidden If the system cursor should be hidden when not shown.
	 */
	void SetCursorHidden(bool hidden);

	/**
	 * Called when a mouse button changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(MouseButton, InputAction, BitMask<InputMod>)> &OnButton() { return onButton; }

	/**
	 * Called when the mouse moves.
	 * @return The delegate.
	 */
	rocket::signal<void(Vector2d)> &OnPosition() { return onPosition; }

	/**
	 * Called when the mouse enters the window.
	 * @return The delegate.
	 */
	rocket::signal<void(bool)> &OnEnter() { return onEnter; }

	/**
	 * Called when the scroll wheel changes.
	 * @return The delegate.
	 */
	rocket::signal<void(Vector2d)> &OnScroll() { return onScroll; }

	/**
	 * Called when a group of files/folders is dropped onto the window.
	 * @return The delegate.
	 */
	rocket::signal<void(std::vector<std::string>)> &OnDrop() { return onDrop; }

private:
	static double SmoothScrollWheel(double value, float delta);

	friend void CallbackMouseButton(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
	friend void CallbackCursorPos(GLFWwindow *window, double xpos, double ypos);
	friend void CallbackCursorEnter(GLFWwindow *window, int32_t entered);
	friend void CallbackScroll(GLFWwindow *window, double xoffset, double yoffset);
	friend void CallbackDrop(GLFWwindow *window, int32_t count, const char **paths);

	std::optional<std::pair<std::filesystem::path, CursorHotspot>> currentCursor;
	std::optional<CursorStandard> currentStandard;
	GLFWcursor *cursor = nullptr;

	Vector2d lastPosition;
	Vector2d position;
	Vector2d positionDelta;
	Vector2d lastScroll;
	Vector2d scroll;
	Vector2d scrollDelta;
	bool windowSelected = false;
	bool cursorHidden = false;

	rocket::signal<void(MouseButton, InputAction, BitMask<InputMod>)> onButton;
	rocket::signal<void(Vector2d)> onPosition;
	rocket::signal<void(bool)> onEnter;
	rocket::signal<void(Vector2d)> onScroll;
	rocket::signal<void(std::vector<std::string>)> onDrop;
};
}
