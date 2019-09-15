#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"
#include "Helpers/EnumClass.hpp"
#include "Maths/Vector2.hpp"
#include "Monitor.hpp"

struct GLFWwindow;

namespace acid {
enum class InputAction :
	int32_t {
	Release = 0,
	Press = 1,
	Repeat = 2
};

ENABLE_BITMASK_OPERATORS(InputAction);

enum class InputMod :
	int32_t {
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
class ACID_EXPORT Window : public Module {
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Window *Get() { return Engine::Get()->GetModule<Window>(); }

	Window();
	~Window();

	void Update() override;

	/**
	 * Gets the size of the window in pixels.
	 * @param checkFullscreen If in fullscreen and true size will be the screens size.
	 * @return The size of the window.
	 */
	const Vector2ui &GetSize(bool checkFullscreen = true) const { return (m_fullscreen && checkFullscreen) ? m_fullscreenSize : m_size; }

	/**
	 * Gets the aspect ratio between the windows width and height.
	 * @return The aspect ratio.
	 */
	float GetAspectRatio() const { return m_aspectRatio; }

	/**
	 * Sets the window size.
	 * @param size The new size in pixels.
	 */
	void SetSize(const Vector2i &size);

	/**
	 * Gets the windows position in pixels.
	 * @return The windows position.
	 */
	const Vector2ui &GetPosition() const { return m_position; }

	/**
	 * Sets the window position to a new position in pixels.
	 * @param position The new position in pixels.
	 */
	void SetPosition(const Vector2i &position);

	/**
	 * Gets the window's title.
	 * @return The window's title.
	 */
	const std::string &GetTitle() const { return m_title; }

	/**
	 * Sets window title.
	 * @param title The new title.
	 */
	void SetTitle(const std::string &title);

	/**
	 * Sets window icon images.
	 * @param filenames The new icon files.
	 */
	void SetIcons(const std::vector<std::string> &filenames);

	/**
	 * Gets weather the window is borderless or not.
	 * @return If the window is borderless.
	 */
	bool IsBorderless() const { return m_borderless; }

	/**
	 * Sets the window to be borderless.
	 * @param borderless Weather or not to be borderless.
	 */
	void SetBorderless(bool borderless);

	/**
	 * Gets weather the window is resizable or not.
	 * @return If the window is resizable.
	 */
	bool IsResizable() const { return m_resizable; }

	/**
	 * Sets the window to be resizable.
	 * @param resizable Weather or not to be resizable.
	 */
	void SetResizable(bool resizable);

	/**
	 * Gets weather the window is floating or not, if floating the window will always display above other windows.
	 * @return If the window is floating.
	 */
	bool IsFloating() const { return m_floating; }

	/**
	 * Sets the window to be floating.
	 * @param floating Weather or not to be floating.
	 */
	void SetFloating(bool floating);

	/**
	 * Gets weather the window is fullscreen or not.
	 * @return Fullscreen or windowed.
	 */
	bool IsFullscreen() const { return m_fullscreen; }

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
	bool IsClosed() const { return m_closed; }

	/**
	 * Gets if the window is selected.
	 * @return If the window is selected.
	 */
	bool IsFocused() const { return m_focused; }

	/**
	 * Gets the windows is minimized.
	 * @return If the window is minimized.
	 */
	bool IsIconified() const { return m_iconified; }

	/**
	 * Sets the window to be iconified (minimized).
	 * @param iconify If the window will be set as iconified.
	 */
	void SetIconified(bool iconify);

	ACID_NO_EXPORT GLFWwindow *GetWindow() const { return m_window; }

	const std::vector<std::unique_ptr<Monitor>> &GetMonitors() const { return m_monitors; };
	
	const Monitor *GetPrimaryMonitor() const;

	/**
	 * Called when the window is resized.
	 * @return The delegate.
	 */
	Delegate<void(Vector2ui)> &OnSize() { return m_onSize; }

	/**
	 * Called when the window is moved.
	 * @return The delegate.
	 */
	Delegate<void(Vector2ui)> &OnPosition() { return m_onPosition; }

	/**
	 * Called when a monitor has been connected or disconnected.
	 * @return The delegate.
	 */
	Delegate<void(Monitor *, bool)> &OnMonitorConnect() { return m_onMonitorConnect; }

	/**
	 * Called when the windows title changed.
	 * @return The delegate.
	 */
	Delegate<void(std::string)> &OnTitle() { return m_onTitle; }

	/**
	 * Called when the window has toggled borderless on or off.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnBorderless() { return m_onBorderless; }

	/**
	 * Called when the window has toggled resizable on or off.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnResizable() { return m_onResizable; }

	/**
	 * Called when the window has toggled floating on or off.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnFloating() { return m_onFloating; }

	/**
	 * Called when the has gone fullscreen or windowed.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnFullscreen() { return m_onFullscreen; }

	/**
	 * Called when the window requests a close.
	 * @return The delegate.
	 */
	Delegate<void()> &OnClose() { return m_onClose; }

	/**
	 * Called when the window is focused or unfocused.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnFocus() { return m_onFocus; }

	/**
	 * Called when the window is minimized or maximized.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnIconify() { return m_onIconify; }

	ACID_NO_EXPORT static std::string StringifyResultGlfw(int32_t result);
	ACID_NO_EXPORT static void CheckGlfw(int32_t result);

	static std::pair<const char **, uint32_t> GetInstanceExtensions();
	VkResult CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const;

private:
	friend void CallbackError(int32_t error, const char *description);
	friend void CallbackMonitor(GLFWmonitor *monitor, int32_t event);
	friend void CallbackPosition(GLFWwindow *window, int32_t xpos, int32_t ypos);
	friend void CallbackSize(GLFWwindow *window, int32_t width, int32_t height);
	friend void CallbackClose(GLFWwindow *window);
	friend void CallbackFocus(GLFWwindow *window, int32_t focused);
	friend void CallbackIconify(GLFWwindow *window, int32_t iconified);
	friend void CallbackFrame(GLFWwindow *window, int32_t width, int32_t height);

	Vector2ui m_size;
	Vector2ui m_fullscreenSize;
	float m_aspectRatio = 1.0f;

	Vector2ui m_position;

	std::string m_title;
	bool m_borderless = false;
	bool m_resizable = false;
	bool m_floating = false;
	bool m_fullscreen = false;

	bool m_closed = false;
	bool m_focused = false;
	bool m_iconified = false;

	GLFWwindow *m_window = nullptr;
	std::vector<std::unique_ptr<Monitor>> m_monitors;

	Delegate<void(Vector2ui)> m_onSize;
	Delegate<void(Vector2ui)> m_onPosition;
	Delegate<void(Monitor *, bool)> m_onMonitorConnect;
	Delegate<void(std::string)> m_onTitle;
	Delegate<void(bool)> m_onBorderless;
	Delegate<void(bool)> m_onResizable;
	Delegate<void(bool)> m_onFloating;
	Delegate<void(bool)> m_onFullscreen;
	Delegate<void()> m_onClose;
	Delegate<void(bool)> m_onFocus;
	Delegate<void(bool)> m_onIconify;
};
}
