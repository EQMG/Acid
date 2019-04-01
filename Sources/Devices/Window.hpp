#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"
#include "Helpers/Delegate.hpp"
#include "Helpers/EnumClass.hpp"
#include "Maths/Vector2.hpp"
#include "Monitor.hpp"

struct GLFWwindow;

namespace acid
{
enum class InputAction :
	int32_t
{
	Release = 0, Press = 1, Repeat = 2
};

enum class InputMod :
	int32_t
{
	None = 0, Shift = 1, Control = 2, Alt = 4, Super = 8
};

ENABLE_BITMASK_OPERATORS(InputMod)

/**
 * @brief Module used for managing a window.
 */
class ACID_EXPORT Window :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Window *Get() { return Engine::Get()->GetModuleManager().Get<Window>(); }

	Window();

	~Window();

	void Update() override;

	/**
	 * Gets the width of the window in pixels.
	 * @return The width of the window.
	 */
	const uint32_t &GetWidth() const { return m_fullscreen ? m_fullscreenWidth : m_windowWidth; }

	/**
	 * Gets the non-fullscreen width of the window in pixels.
	 * @return The width of the window.
	 */
	const uint32_t &GetWindowWidth() const { return m_windowWidth; }

	/**
	 * Sets the width of the window in pixels.
	 * @param width The new width in pixels.
	 */
	void SetWidth(const uint32_t &width) { SetDimensions(width, GetHeight()); }

	/**
	 * Gets the height of the window in pixels.
	 * @return The height of the window.
	 */
	const uint32_t &GetHeight() const { return m_fullscreen ? m_fullscreenHeight : m_windowHeight; }

	/**
	 * Gets the non-fullscreen height of the window in pixels.
	 * @return The height of the window.
	 */
	const uint32_t &GetWindowHeight() const { return m_windowHeight; }

	/**
	 * Sets the height of the window in pixels.
	 * @param height The new height in pixels.
	 */
	void SetHeight(const uint32_t &height) { SetDimensions(GetWidth(), height); }

	/**
	 * Gets the aspect ratio between the windows width and height.
	 * @return The aspect ratio.
	 */
	const float &GetAspectRatio() const { return m_aspectRatio; }

	/**
	 * Gets the dimensions of the window in pixels.
	 * @return The dimensions of the window.
	 */
	Vector2 GetDimensions() const { return Vector2(static_cast<float>(GetWidth()), static_cast<float>(GetHeight())); }

	/**
	 * Sets the window size to a new size.
	 * @param width The new width in pixels.
	 * @param height The new height in pixels.
	 */
	void SetDimensions(const uint32_t &width, const uint32_t &height);

	/**
	 * Sets the window size to a new size.
	 * @param size The new width in pixels.
	 */
	void SetDimensions(const Vector2 &size);

	/**
	 * Gets the windows x position in pixels.
	 * @return The windows x position.
	 */
	const uint32_t &GetPositionX() const { return m_positionX; }

	/**
	 * Gets the windows y position in pixels.
	 * @return The windows y position.
	 */
	const uint32_t &GetPositionY() const { return m_positionY; }

	/**
	 * Gets the windows position in pixels.
	 * @return The dimension of the window.
	 */
	Vector2 GetPosition() const { return Vector2(static_cast<float>(m_positionX), static_cast<float>(m_positionY)); }

	/**
	 * Sets the window position to a new position in pixels.
	 * @param x The new x position in pixels.
	 * @param y The new y position in pixels.
	 */
	void SetPosition(const uint32_t &x, const uint32_t &y);

	/**
	 * Sets the window position to a new position in pixels.
	 * @param position The new position in pixels.
	 */
	void SetPosition(const Vector2 &position);

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
	const bool &IsBorderless() const { return m_borderless; }

	/**
	 * Sets the window to be borderless.
	 * @param borderless Weather or not to be borderless.
	 */
	void SetBorderless(const bool &borderless);

	/**
	 * Gets weather the window is resizable or not.
	 * @return If the window is resizable.
	 */
	const bool &IsResizable() const { return m_resizable; }

	/**
	 * Sets the window to be resizable.
	 * @param resizable Weather or not to be resizable.
	 */
	void SetResizable(const bool &resizable);

	/**
	 * Gets weather the window is floating or not, if floating the window will always display above other windows.
	 * @return If the window is floating.
	 */
	const bool &IsFloating() const { return m_floating; }

	/**
	 * Sets the window to be floating.
	 * @param floating Weather or not to be floating.
	 */
	void SetFloating(const bool &floating);

	/**
	 * Gets weather the window is fullscreen or not.
	 * @return Fullscreen or windowed.
	 */
	const bool &IsFullscreen() const { return m_fullscreen; }

	/**
	 * Sets the window to be fullscreen or windowed.
	 * @param fullscreen If the window will be fullscreen.
	 * @param monitor The monitor to display in.
	 */
	void SetFullscreen(const bool &fullscreen, const std::optional<Monitor> &monitor = {});

	/**
	 * Gets if the window is closed.
	 * @return If the window is closed.
	 */
	const bool &IsClosed() const { return m_closed; }

	/**
	 * Gets if the window is selected.
	 * @return If the window is selected.
	 */
	const bool &IsFocused() const { return m_focused; }

	/**
	 * Gets the windows is minimized.
	 * @return If the window is minimized.
	 */
	const bool &IsIconified() const { return m_iconified; }

	/**
	 * Sets the window to be iconified (minimized).
	 * @param iconify If the window will be set as iconified.
	 */
	void SetIconified(const bool &iconify);

	ACID_HIDDEN GLFWwindow *GetWindow() const { return m_window; }

	const std::vector<Monitor> &GetMonitors() const { return m_monitors; };

	/**
	 * Called when the window is resized.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, uint32_t)> &OnDimensions() { return m_onDimensions; }

	/**
	 * Called when the window is moved.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, uint32_t)> &OnPosition() { return m_onPosition; }

	/**
	 * Called when a monitor has been connected or disconnected.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t, bool)> &OnMonitorConnect() { return m_onMonitorConnect; }

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

	ACID_HIDDEN static std::string StringifyResultGlfw(const int32_t &result);

	ACID_HIDDEN static void CheckGlfw(const int32_t &result);

	std::pair<const char **, uint32_t> GetInstanceExtensions() const;

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

	uint32_t m_windowWidth;
	uint32_t m_windowHeight;
	uint32_t m_fullscreenWidth;
	uint32_t m_fullscreenHeight;
	float m_aspectRatio;

	uint32_t m_positionX;
	uint32_t m_positionY;

	std::string m_title;
	bool m_borderless;
	bool m_resizable;
	bool m_floating;
	bool m_fullscreen;

	bool m_closed;
	bool m_focused;
	bool m_iconified;

	GLFWwindow *m_window;
	std::vector<Monitor> m_monitors;

	Delegate<void(uint32_t, uint32_t)> m_onDimensions;
	Delegate<void(uint32_t, uint32_t)> m_onPosition;
	Delegate<void(uint32_t, bool)> m_onMonitorConnect;
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
