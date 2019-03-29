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

/// <summary>
/// A module used for the creation, updating and destruction of the display.
/// </summary>
class ACID_EXPORT Window :
	public Module
{
public:
	/// <summary>
	/// Gets this engine instance.
	/// </summary>
	/// <returns> The current module instance. </returns>
	static Window *Get() { return Engine::Get()->GetModuleManager().Get<Window>(); }

	Window();

	~Window();

	void Update() override;

	/// <summary>
	/// Gets the width of the window in pixels.
	/// </summary>
	/// <returns> The width of the window. </returns>
	const uint32_t &GetWidth() const { return m_fullscreen ? m_fullscreenWidth : m_windowWidth; }

	/// <summary>
	/// Gets the non-fullscreen width of the window in pixels.
	/// </summary>
	/// <returns> The width of the window. </returns>
	const uint32_t &GetWindowWidth() const { return m_windowWidth; }

	/// <summary>
	/// Sets the width of the window in pixels.
	/// </summary>
	/// <param name="width"> The new width in pixels. </param>
	void SetWidth(const uint32_t &width) { SetDimensions(width, GetHeight()); }

	/// <summary>
	/// Gets the height of the window in pixels.
	/// </summary>
	/// <returns> The height of the window. </returns>
	const uint32_t &GetHeight() const { return m_fullscreen ? m_fullscreenHeight : m_windowHeight; }

	/// <summary>
	/// Gets the non-fullscreen height of the window in pixels.
	/// </summary>
	/// <returns> The height of the window. </returns>
	const uint32_t &GetWindowHeight() const { return m_windowHeight; }

	/// <summary>
	/// Sets the height of the window in pixels.
	/// </summary>
	/// <param name="height"> The new height in pixels. </param>
	void SetHeight(const uint32_t &height) { SetDimensions(GetWidth(), height); }

	/// <summary>
	/// Gets the aspect ratio between the windows width and height.
	/// </summary>
	/// <returns> The aspect ratio. </returns>
	const float &GetAspectRatio() const { return m_aspectRatio; }

	/// <summary>
	/// Gets the dimensions of the window in pixels.
	/// </summary>
	/// <returns> The dimensions of the window. </returns>
	Vector2 GetDimensions() const { return Vector2(static_cast<float>(GetWidth()), static_cast<float>(GetHeight())); }

	/// <summary>
	/// Sets the window size to a new size.
	/// </summary>
	/// <param name="width"> The new width in pixels. </param>
	/// <param name="height"> The new height in pixels. </param>
	void SetDimensions(const uint32_t &width, const uint32_t &height);

	/// <summary>
	/// Sets the window size to a new size.
	/// </summary>
	/// <param name="size"> The new width in pixels. </param>
	void SetDimensions(const Vector2 &size);

	/// <summary>
	/// Gets the windows Y position in pixels.
	/// </summary>
	/// <returns> The windows x position. </returns>
	const uint32_t &GetPositionX() const { return m_positionX; }

	/// <summary>
	/// Gets the windows Y position in pixels.
	/// </summary>
	/// <returns> The windows Y position. </returns>
	const uint32_t &GetPositionY() const { return m_positionY; }

	/// <summary>
	/// Gets the windows position in pixels.
	/// </summary>
	/// <returns> The dimension of the window. </returns>
	Vector2 GetPosition() const { return Vector2(static_cast<float>(m_positionX), static_cast<float>(m_positionY)); }

	/// <summary>
	/// Sets the window position to a new position in pixels.
	/// </summary>
	/// <param name="x"> The new x position in pixels. </param>
	/// <param name="y"> The new y position in pixels. </param>
	void SetPosition(const uint32_t &x, const uint32_t &y);

	/// <summary>
	/// Sets the window position to a new position in pixels.
	/// </summary>
	/// <param name="position"> The new position in pixels. </param>
	void SetPosition(const Vector2 &position);

	/// <summary>
	/// Gets the window's title.
	/// </summary>
	/// <returns> The window's title. </returns>
	const std::string &GetTitle() const { return m_title; }

	/// <summary>
	/// Sets window title
	/// </summary>
	/// <param name="title"> The new title. </param>
	void SetTitle(const std::string &title);

	/// <summary>
	/// Sets window icon images.
	/// </summary>
	/// <param name="filenames"> The new icon files. </param>
	void SetIcons(const std::vector<std::string> &filenames);

	/// <summary>
	/// Gets weather the window is borderless or not.
	/// </summary>
	/// <returns> If the window is borderless. </returns>
	const bool &IsBorderless() const { return m_borderless; }

	/// <summary>
	/// Sets the window to be borderless.
	/// </summary>
	/// <param name="borderless"> Weather or not to be borderless. </param>
	void SetBorderless(const bool &borderless);

	/// <summary>
	/// Gets weather the window is resizable or not.
	/// </summary>
	/// <returns> If the window is resizable. </returns>
	const bool &IsResizable() const { return m_resizable; }

	/// <summary>
	/// Sets the window to be resizable.
	/// </summary>
	/// <param name="resizable"> Weather or not to be resizable. </param>
	void SetResizable(const bool &resizable);

	/// <summary>
	/// Gets weather the window is floating or not, if floating the window will always display above other windows.
	/// </summary>
	/// <returns> If the window is floating. </returns>
	const bool &IsFloating() const { return m_floating; }

	/// <summary>
	/// Sets the window to be floating.
	/// </summary>
	/// <param name="floating"> Weather or not to be floating. </param>
	void SetFloating(const bool &floating);

	/// <summary>
	/// Gets weather the window is fullscreen or not.
	/// </summary>
	/// <returns> Fullscreen or windowed. </returns>
	const bool &IsFullscreen() const { return m_fullscreen; }

	/// <summary>
	/// Sets the window to be fullscreen or windowed.
	/// </summary>
	/// <param name="fullscreen"> If the window will be fullscreen. </param>
	/// <param name="monitor"> The monitor to display in. </param>
	void SetFullscreen(const bool &fullscreen, const std::optional<Monitor> &monitor = {});

	/// <summary>
	/// Gets if the window is closed.
	/// </summary>
	/// <returns> If the window is closed. </returns>
	const bool &IsClosed() const { return m_closed; }

	/// <summary>
	/// Gets if the window is selected.
	/// </summary>
	/// <returns> If the window is selected. </returns>
	const bool &IsFocused() const { return m_focused; }

	/// <summary>
	/// Gets the windows is minimized.
	/// </summary>
	/// <returns> If the window is minimized. </returns>
	const bool &IsIconified() const { return m_iconified; }

	/// <summary>
	/// Sets the window to be iconified (minimized).
	/// </summary>
	/// <param name="iconify"> If the window will be set as iconified. </param>
	void SetIconified(const bool &iconify);

	ACID_HIDDEN GLFWwindow *GetWindow() const { return m_window; }

	const std::vector<Monitor> &GetMonitors() const { return m_monitors; };

	Delegate<void(uint32_t, uint32_t)> &GetOnDimensions() { return m_onDimensions; }

	Delegate<void(uint32_t, uint32_t)> &GetOnPosition() { return m_onPosition; }

	Delegate<void(uint32_t, bool)> &GetOnMonitorConnect() { return m_onMonitorConnect; }

	Delegate<void(std::string)> &GetOnTitle() { return m_onTitle; }

	Delegate<void(bool)> &GetOnBorderless() { return m_onBorderless; }

	Delegate<void(bool)> &GetOnResizable() { return m_onResizable; }

	Delegate<void(bool)> &GetOnFloating() { return m_onFloating; }

	Delegate<void(bool)> &GetOnFullscreen() { return m_onFullscreen; }

	Delegate<void()> &GetOnClose() { return m_onClose; }

	Delegate<void(bool)> &GetOnFocus() { return m_onFocus; }

	Delegate<void(bool)> &GetOnIconify() { return m_onIconify; }

	ACID_HIDDEN static std::string StringifyResultGlfw(const int32_t &result);

	ACID_HIDDEN static void CheckGlfw(const int32_t &result);

	std::pair<const char **, uint32_t> GetInstanceExtensions() const;

	VkResult CreateSurface(const VkInstance &instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) const;

private:
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

	friend void CallbackError(int32_t error, const char *description);

	friend void CallbackMonitor(GLFWmonitor *monitor, int32_t event);

	friend void CallbackPosition(GLFWwindow *window, int32_t xpos, int32_t ypos);

	friend void CallbackSize(GLFWwindow *window, int32_t width, int32_t height);

	friend void CallbackClose(GLFWwindow *window);

	friend void CallbackFocus(GLFWwindow *window, int32_t focused);

	friend void CallbackIconify(GLFWwindow *window, int32_t iconified);

	friend void CallbackFrame(GLFWwindow *window, int32_t width, int32_t height);
};
}
