#pragma once

#include <volk.h>
#include <bitmask.hpp>
#include <rocket.hpp>

#include "Maths/Vector2.hpp"
#include "Cursor.hpp"
#include "Monitor.hpp"

struct GLFWwindow;

namespace acid {
enum class Key : int16_t {
	Unknown = -1,
	Space = 32,
	Apostrophe = 39,
	Comma = 44,
	Minus = 45,
	Period = 46,
	Slash = 47,
	_0 = 48,
	_1 = 49,
	_2 = 50,
	_3 = 51,
	_4 = 52,
	_5 = 53,
	_6 = 54,
	_7 = 55,
	_8 = 56,
	_9 = 57,
	Semicolon = 59,
	Equal = 61,
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	LeftBracket = 91,
	Backslash = 92,
	RightBracket = 93,
	GraveAccent = 96,
	World1 = 161,
	World2 = 162,
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	PageUp = 266,
	PageDown = 267,
	Home = 268,
	End = 269,
	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
	PrintScreen = 283,
	Pause = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,
	Numpad0 = 320,
	Numpad1 = 321,
	Numpad2 = 322,
	Numpad3 = 323,
	Numpad4 = 324,
	Numpad5 = 325,
	Numpad6 = 326,
	Numpad7 = 327,
	Numpad8 = 328,
	Numpad9 = 329,
	NumpadDecimal = 330,
	NumpadDivide = 331,
	NumpadMultiply = 332,
	NumpadSubtract = 333,
	NumpadAdd = 334,
	NumpadEnter = 335,
	NumpadEqual = 336,
	ShiftLeft = 340,
	ControlLeft = 341,
	AltLeft = 342,
	SuperLeft = 343,
	ShiftRight = 344,
	ControlRight = 345,
	AltRight = 346,
	SuperRight = 347,
	Menu = 348
};

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
	Middle = _3
};

enum class InputAction : int32_t {
	Release = 0,
	Press = 1,
	Repeat = 2
};
ENABLE_BITMASK_OPERATORS(InputAction)

enum class InputMod : int32_t {
	None = 0,
	Shift = 1,
	Control = 2,
	Alt = 4,
	Super = 8,
	CapsLock = 16,
	NumLock = 32
};
ENABLE_BITMASK_OPERATORS(InputMod)

using WindowId = std::size_t;

class Window : NonCopyable {
public:
	Window(std::size_t id);
	~Window();

	void Update();

	/**
	 * Gets the size of the window in pixels.
	 * @param checkFullscreen If in fullscreen and true size will be the screens size.
	 * @return The size of the window.
	 */
	const Vector2ui &GetSize(bool checkFullscreen = true) const { return (fullscreen && checkFullscreen) ? fullscreenSize : size; }

	/**
	 * Sets the window size.
	 * @param size The new size in pixels.
	 */
	void SetSize(const Vector2i &size);

	/**
	 * Gets the aspect ratio between the windows width and height.
	 * @return The aspect ratio.
	 */
	float GetAspectRatio() const { return static_cast<float>(GetSize().x) / static_cast<float>(GetSize().y); }

	/**
	 * Gets the windows position in pixels.
	 * @return The windows position.
	 */
	const Vector2ui &GetPosition() const { return position; }

	/**
	 * Sets the window position to a new position in pixels.
	 * @param position The new position in pixels.
	 */
	void SetPosition(const Vector2i &position);

	/**
	 * Gets the window's title.
	 * @return The window's title.
	 */
	const std::string &GetTitle() const { return title; }

	/**
	 * Sets window title.
	 * @param title The new title.
	 */
	void SetTitle(const std::string &title);

	/**
	 * Sets window icon images.
	 * @param filenames The new icon files.
	 */
	void SetIcons(const std::vector<std::filesystem::path> &filenames);

	/**
	 * Gets weather the window is borderless or not.
	 * @return If the window is borderless.
	 */
	bool IsBorderless() const { return borderless; }

	/**
	 * Sets the window to be borderless.
	 * @param borderless Weather or not to be borderless.
	 */
	void SetBorderless(bool borderless);

	/**
	 * Gets weather the window is resizable or not.
	 * @return If the window is resizable.
	 */
	bool IsResizable() const { return resizable; }

	/**
	 * Sets the window to be resizable.
	 * @param resizable Weather or not to be resizable.
	 */
	void SetResizable(bool resizable);

	/**
	 * Gets weather the window is floating or not, if floating the window will always display above other windows.
	 * @return If the window is floating.
	 */
	bool IsFloating() const { return floating; }

	/**
	 * Sets the window to be floating.
	 * @param floating Weather or not to be floating.
	 */
	void SetFloating(bool floating);

	/**
	 * Gets weather the window is fullscreen or not.
	 * @return Fullscreen or windowed.
	 */
	bool IsFullscreen() const { return fullscreen; }

	/**
	 * Sets the window to be fullscreen or windowed.
	 * @param fullscreen If the window will be fullscreen.
	 * @param monitor The monitor to display in.
	 */
	void SetFullscreen(bool fullscreen, Monitor *monitor = nullptr);

	/**
	 * Gets if the window is closed.
	 * @return If the window is closed.
	 */
	bool IsClosed() const { return closed; }

	/**
	 * Gets if the window is selected.
	 * @return If the window is selected.
	 */
	bool IsFocused() const { return focused; }

	/**
	 * Gets the windows is minimized.
	 * @return If the window is minimized.
	 */
	bool IsIconified() const { return iconified; }

	/**
	 * Sets the window to be iconified (minimized).
	 * @param iconify If the window will be set as iconified.
	 */
	void SetIconified(bool iconify);

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
	 * Sets the cursor new style.
	 * @param cursor The cursor style to use.
	 */
	void SetCursor(const Cursor *cursor);

	/**
	 * Gets the current state of a key.
	 * @param key The key to get the state of.
	 * @return The keys state.
	 */
	InputAction GetKey(Key key) const;

	/**
	 * Gets the current state of a mouse button.
	 * @param mouseButton The mouse button to get the state of.
	 * @return The mouse buttons state.
	 */
	InputAction GetMouseButton(MouseButton mouseButton) const;

	/**
	 * Gets the mouses position.
	 * @return The mouses position.
	 */
	const Vector2d &GetMousePosition() const { return mousePosition; }

	/**
	 * Sets the mouse position.
	 * @param position The new mouse position.
	 */
	void SetMousePosition(const Vector2d &mousePosition);

	/**
	 * Gets the mouse position delta.
	 * @return The mouse position delta.
	 */
	const Vector2d &GetMousePositionDelta() const { return mousePositionDelta; }

	/**
	 * Gets the mouses virtual scroll position.
	 * @return The mouses virtual scroll position.
	 */
	const Vector2d &GetMouseScroll() const { return mouseScroll; }

	/**
	 * Sets the mouse virtual scroll position.
	 * @param scroll The new mouse virtual scroll position.
	 */
	void SetMouseScroll(const Vector2d &scroll);

	/**
	 * Gets the mouse scroll delta.
	 * @return The mouse scroll delta.
	 */
	const Vector2d &GetMouseScrollDelta() const { return mouseScrollDelta; }

	static std::string ToString(Key key);

	ACID_NO_EXPORT GLFWwindow *GetWindow() const { return window; }

	const Monitor *GetCurrentMonitor() const;

	VkResult CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const;

	/**
	 * Called when the window is resized.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(Vector2ui)> &OnSize() { return onSize; }

	/**
	 * Called when the window is moved.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(Vector2ui)> &OnPosition() { return onPosition; }

	/**
	 * Called when the windows title changed.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(std::string)> &OnTitle() { return onTitle; }

	/**
	 * Called when the window has toggled borderless on or off.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(bool)> &OnBorderless() { return onBorderless; }

	/**
	 * Called when the window has toggled resizable on or off.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(bool)> &OnResizable() { return onResizable; }

	/**
	 * Called when the window has toggled floating on or off.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(bool)> &OnFloating() { return onFloating; }

	/**
	 * Called when the has gone fullscreen or windowed.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(bool)> &OnFullscreen() { return onFullscreen; }

	/**
	 * Called when the window requests a close.
	 * @return The rocket::signal.
	 */
	rocket::signal<void()> &OnClose() { return onClose; }

	/**
	 * Called when the window is focused or unfocused.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(bool)> &OnFocus() { return onFocus; }

	/**
	 * Called when the window is minimized or maximized.
	 * @return The rocket::signal.
	 */
	rocket::signal<void(bool)> &OnIconify() { return onIconify; }

	/**
	 * Called when the mouse enters the window.
	 * @return The delegate.
	 */
	rocket::signal<void(bool)> &OnEnter() { return onEnter; }

	/**
	 * Called when a group of files/folders is dropped onto the window.
	 * @return The delegate.
	 */
	rocket::signal<void(std::vector<std::string>)> &OnDrop() { return onDrop; }

	/**
	 * Called when a key changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(Key, InputAction, bitmask::bitmask<InputMod>)> &OnKey() { return onKey; }

	/**
	 * Called when a character has been typed.
	 * @return The delegate.
	 */
	rocket::signal<void(char)> &OnChar() { return onChar; }

	/**
	 * Called when a mouse button changes state.
	 * @return The delegate.
	 */
	rocket::signal<void(MouseButton, InputAction, bitmask::bitmask<InputMod>)> &OnMouseButton() { return onMouseButton; }

	/**
	 * Called when the mouse moves.
	 * @return The delegate.
	 */
	rocket::signal<void(Vector2d)> &OnMousePosition() { return onMousePosition; }

	/**
	 * Called when the scroll wheel changes.
	 * @return The delegate.
	 */
	rocket::signal<void(Vector2d)> &OnMouseScroll() { return onMouseScroll; }

private:
	friend void CallbackWindowPosition(GLFWwindow *glfwWindow, int32_t xpos, int32_t ypos);
	friend void CallbackWindowSize(GLFWwindow *glfwWindow, int32_t width, int32_t height);
	friend void CallbackWindowClose(GLFWwindow *glfwWindow);
	friend void CallbackWindowFocus(GLFWwindow *glfwWindow, int32_t focused);
	friend void CallbackWindowIconify(GLFWwindow *glfwWindow, int32_t iconified);
	friend void CallbackFramebufferSize(GLFWwindow *glfwWindow, int32_t width, int32_t height);
	friend void CallbackCursorEnter(GLFWwindow *glfwWindow, int32_t entered);
	friend void CallbackDrop(GLFWwindow *glfwWindow, int32_t count, const char **paths);
	friend void CallbackKey(GLFWwindow *glfwWindow, int32_t key, int32_t scancode, int32_t action, int32_t mods);
	friend void CallbackChar(GLFWwindow *glfwWindow, uint32_t codepoint);
	friend void CallbackMouseButton(GLFWwindow *glfwWindow, int32_t button, int32_t action, int32_t mods);
	friend void CallbackCursorPos(GLFWwindow *glfwWindow, double xpos, double ypos);
	friend void CallbackScroll(GLFWwindow *glfwWindow, double xoffset, double yoffset);

	static double SmoothScrollWheel(double value, float delta);

	std::size_t id;
	GLFWwindow *window = nullptr;

	Vector2ui size;
	Vector2ui fullscreenSize;

	Vector2ui position;

	std::string title;
	bool borderless = false;
	bool resizable = false;
	bool floating = false;
	bool fullscreen = false;

	bool closed = false;
	bool focused = false;
	bool iconified = false;

	bool windowSelected = false;
	bool cursorHidden = false;

	Vector2d mouseLastPosition;
	Vector2d mousePosition;
	Vector2d mousePositionDelta;
	Vector2d mouseLastScroll;
	Vector2d mouseScroll;
	Vector2d mouseScrollDelta;

	rocket::signal<void(Vector2ui)> onSize;
	rocket::signal<void(Vector2ui)> onPosition;
	rocket::signal<void(std::string)> onTitle;
	rocket::signal<void(bool)> onBorderless;
	rocket::signal<void(bool)> onResizable;
	rocket::signal<void(bool)> onFloating;
	rocket::signal<void(bool)> onFullscreen;
	rocket::signal<void()> onClose;
	rocket::signal<void(bool)> onFocus;
	rocket::signal<void(bool)> onIconify;
	rocket::signal<void(bool)> onEnter;
	rocket::signal<void(std::vector<std::string>)> onDrop;
	rocket::signal<void(Key, InputAction, bitmask::bitmask<InputMod>)> onKey;
	rocket::signal<void(char)> onChar;
	rocket::signal<void(MouseButton, InputAction, bitmask::bitmask<InputMod>)> onMouseButton;
	rocket::signal<void(Vector2d)> onMousePosition;
	rocket::signal<void(Vector2d)> onMouseScroll;
};
}

namespace magic_enum::customize {
template<>
struct enum_range<acid::Key> {
	inline constexpr static int min = -1;
	inline constexpr static int max = 400;
};
}
