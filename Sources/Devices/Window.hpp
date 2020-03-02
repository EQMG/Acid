#pragma once

#include <volk.h>

#include "Engine/Engine.hpp"
#include "Utils/Delegate.hpp"
#include "Utils/EnumClass.hpp"
#include "Maths/Vector2.hpp"
#include "Monitor.hpp"

struct GLFWwindow;

namespace acid {
enum class InputAction : int32_t {
	Release = 0,
	Press = 1,
	Repeat = 2
};

ENABLE_BITMASK_OPERATORS(InputAction);

enum class InputMod : int32_t {
	None = 0,
	Shift = 1,
	Control = 2,
	Alt = 4,
	Super = 8
};

ENABLE_BITMASK_OPERATORS(InputMod);

/**
 * @brief Module used for managing a window.
 */
class ACID_EXPORT Window : public Module::Registrar<Window, Module::Stage::Pre> {
public:
	Window();
	~Window();

	void Update() override;

	/**
	 * Gets the size of the window in pixels.
	 * @param checkFullscreen If in fullscreen and true size will be the screens size.
	 * @return The size of the window.
	 */
	const Vector2ui &GetSize(bool checkFullscreen = true) const { return (fullscreen && checkFullscreen) ? fullscreenSize : size; }

	/**
	 * Gets the aspect ratio between the windows width and height.
	 * @return The aspect ratio.
	 */
	float GetAspectRatio() const { return static_cast<float>(GetSize().x) / static_cast<float>(GetSize().y); }

	/**
	 * Sets the window size.
	 * @param size The new size in pixels.
	 */
	void SetSize(const Vector2i &size);

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

	ACID_NO_EXPORT GLFWwindow *GetWindow() const { return window; }

	const std::vector<std::unique_ptr<Monitor>> &GetMonitors() const { return monitors; };

	const Monitor *GetPrimaryMonitor() const;
	const Monitor *GetCurrentMonitor() const;

	/**
	 * Called when the window is resized.
	 * @return The delegate.
	 */
	Delegate<void(Vector2ui)> &OnSize() { return onSize; }

	/**
	 * Called when the window is moved.
	 * @return The delegate.
	 */
	Delegate<void(Vector2ui)> &OnPosition() { return onPosition; }

	/**
	 * Called when a monitor has been connected or disconnected.
	 * @return The delegate.
	 */
	Delegate<void(Monitor *, bool)> &OnMonitorConnect() { return onMonitorConnect; }

	/**
	 * Called when the windows title changed.
	 * @return The delegate.
	 */
	Delegate<void(std::string)> &OnTitle() { return onTitle; }

	/**
	 * Called when the window has toggled borderless on or off.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnBorderless() { return onBorderless; }

	/**
	 * Called when the window has toggled resizable on or off.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnResizable() { return onResizable; }

	/**
	 * Called when the window has toggled floating on or off.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnFloating() { return onFloating; }

	/**
	 * Called when the has gone fullscreen or windowed.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnFullscreen() { return onFullscreen; }

	/**
	 * Called when the window requests a close.
	 * @return The delegate.
	 */
	Delegate<void()> &OnClose() { return onClose; }

	/**
	 * Called when the window is focused or unfocused.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnFocus() { return onFocus; }

	/**
	 * Called when the window is minimized or maximized.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnIconify() { return onIconify; }

	ACID_NO_EXPORT static std::string StringifyResultGlfw(int32_t result);
	ACID_NO_EXPORT static void CheckGlfw(int32_t result);

	std::pair<const char **, uint32_t> GetInstanceExtensions() const;
	VkResult CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const;

private:
	friend void CallbackError(int32_t error, const char *description);
	friend void CallbackMonitor(GLFWmonitor *monitor, int32_t event);
	friend void CallbackWindowPosition(GLFWwindow *window, int32_t xpos, int32_t ypos);
	friend void CallbackWindowSize(GLFWwindow *window, int32_t width, int32_t height);
	friend void CallbackWindowClose(GLFWwindow *window);
	friend void CallbackWindowFocus(GLFWwindow *window, int32_t focused);
	friend void CallbackWindowIconify(GLFWwindow *window, int32_t iconified);
	friend void CallbackFramebufferSize(GLFWwindow *window, int32_t width, int32_t height);

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

	GLFWwindow *window = nullptr;
	std::vector<std::unique_ptr<Monitor>> monitors;

	Delegate<void(Vector2ui)> onSize;
	Delegate<void(Vector2ui)> onPosition;
	Delegate<void(Monitor *, bool)> onMonitorConnect;
	Delegate<void(std::string)> onTitle;
	Delegate<void(bool)> onBorderless;
	Delegate<void(bool)> onResizable;
	Delegate<void(bool)> onFloating;
	Delegate<void(bool)> onFullscreen;
	Delegate<void()> onClose;
	Delegate<void(bool)> onFocus;
	Delegate<void(bool)> onIconify;
};
}
