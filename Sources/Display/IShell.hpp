#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include "Engine/Engine.hpp"

namespace fl
{
	enum MouseButton
	{
		MOUSE_BUTTON_1 = 0,
		MOUSE_BUTTON_2 = 1,
		MOUSE_BUTTON_3 = 2,
		MOUSE_BUTTON_4 = 3,
		MOUSE_BUTTON_5 = 4,
		MOUSE_BUTTON_6 = 5,
		MOUSE_BUTTON_7 = 6,
		MOUSE_BUTTON_8 = 7,
		MOUSE_BUTTON_LEFT = 0,
		MOUSE_BUTTON_RIGHT = 1,
		MOUSE_BUTTON_MIDDLE = 2,
		MOUSE_BUTTON_BEGIN_RANGE = MOUSE_BUTTON_1,
		MOUSE_BUTTON_END_RANGE = MOUSE_BUTTON_8,
		MOUSE_BUTTON_RANGE_SIZE = (MOUSE_BUTTON_8 - MOUSE_BUTTON_1 + 1),
		MOUSE_BUTTON_MAX_ENUM = 0x7FFFFFFF
	};

	enum Key
	{
		KEY_UNKNOWN = -1,
		KEY_SPACE = 32,
		KEY_APOSTROPHE = 39,
		KEY_COMMA = 44,
		KEY_MINUS = 45,
		KEY_PERIOD = 46,
		KEY_SLASH = 47,
		KEY_0 = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,
		KEY_SEMICOLON = 59,
		KEY_EQUAL = 61,
		KEY_A = 65,
		KEY_B = 66,
		KEY_C = 67,
		KEY_D = 68,
		KEY_E = 69,
		KEY_F = 70,
		KEY_G = 71,
		KEY_H = 72,
		KEY_I = 73,
		KEY_J = 74,
		KEY_K = 75,
		KEY_L = 76,
		KEY_M = 77,
		KEY_N = 78,
		KEY_O = 79,
		KEY_P = 80,
		KEY_Q = 81,
		KEY_R = 82,
		KEY_S = 83,
		KEY_T = 84,
		KEY_U = 85,
		KEY_V = 86,
		KEY_W = 87,
		KEY_X = 88,
		KEY_Y = 89,
		KEY_Z = 90,
		KEY_LEFT_BRACKET = 91,
		KEY_BACKSLASH = 92,
		KEY_RIGHT_BRACKET = 93,
		KEY_GRAVE_ACCENT = 96,
		KEY_WORLD_1 = 161,
		KEY_WORLD_2 = 162,
		KEY_ESCAPE = 256,
		KEY_ENTER = 257,
		KEY_TAB = 258,
		KEY_BACKSPACE = 259,
		KEY_INSERT = 260,
		KEY_DELETE = 261,
		KEY_RIGHT = 262,
		KEY_LEFT = 263,
		KEY_DOWN = 264,
		KEY_UP = 265,
		KEY_PAGE_UP = 266,
		KEY_PAGE_DOWN = 267,
		KEY_HOME = 268,
		KEY_END = 269,
		KEY_CAPS_LOCK = 280,
		KEY_SCROLL_LOCK = 281,
		KEY_NUM_LOCK = 282,
		KEY_PRINT_SCREEN = 283,
		KEY_PAUSE = 284,
		KEY_F1 = 290,
		KEY_F2 = 291,
		KEY_F3 = 292,
		KEY_F4 = 293,
		KEY_F5 = 294,
		KEY_F6 = 295,
		KEY_F7 = 296,
		KEY_F8 = 297,
		KEY_F9 = 298,
		KEY_F10 = 299,
		KEY_F11 = 300,
		KEY_F12 = 301,
		KEY_F13 = 302,
		KEY_F14 = 303,
		KEY_F15 = 304,
		KEY_F16 = 305,
		KEY_F17 = 306,
		KEY_F18 = 307,
		KEY_F19 = 308,
		KEY_F20 = 309,
		KEY_F21 = 310,
		KEY_F22 = 311,
		KEY_F23 = 312,
		KEY_F24 = 313,
		KEY_F25 = 314,
		KEY_PAD_0 = 320,
		KEY_PAD_1 = 321,
		KEY_PAD_2 = 322,
		KEY_PAD_3 = 323,
		KEY_PAD_4 = 324,
		KEY_PAD_5 = 325,
		KEY_PAD_6 = 326,
		KEY_PAD_7 = 327,
		KEY_PAD_8 = 328,
		KEY_PAD_9 = 329,
		KEY_PAD_DECIMAL = 330,
		KEY_PAD_DIVIDE = 331,
		KEY_PAD_MULTIPLY = 332,
		KEY_PAD_SUBTRACT = 333,
		KEY_PAD_ADD = 334,
		KEY_PAD_ENTER = 335,
		KEY_PAD_EQUAL = 336,
		KEY_LEFT_SHIFT = 340,
		KEY_LEFT_CONTROL = 341,
		KEY_LEFT_ALT = 342,
		KEY_LEFT_SUPER = 343,
		KEY_RIGHT_SHIFT = 344,
		KEY_RIGHT_CONTROL = 345,
		KEY_RIGHT_ALT = 346,
		KEY_RIGHT_SUPER = 347,
		KEY_MENU = 348,
		KEY_BEGIN_RANGE = KEY_UNKNOWN,
		KEY_END_RANGE = KEY_MENU,
		KEY_RANGE_SIZE = (KEY_MENU - KEY_UNKNOWN + 1),
		KEY_MAX_ENUM = 0x7FFFFFFF
	};

	enum JoystickPort
	{
		JOYSTICK_1 = 0,
		JOYSTICK_2 = 1,
		JOYSTICK_3 = 2,
		JOYSTICK_4 = 3,
		JOYSTICK_5 = 4,
		JOYSTICK_6 = 5,
		JOYSTICK_7 = 6,
		JOYSTICK_8 = 7,
		JOYSTICK_9 = 8,
		JOYSTICK_10 = 9,
		JOYSTICK_11 = 10,
		JOYSTICK_12 = 11,
		JOYSTICK_13 = 12,
		JOYSTICK_14 = 13,
		JOYSTICK_15 = 14,
		JOYSTICK_16 = 15,
		JOYSTICK_BEGIN_RANGE = JOYSTICK_1,
		JOYSTICK_END_RANGE = JOYSTICK_16,
		JOYSTICK_RANGE_SIZE = (JOYSTICK_16 - JOYSTICK_1 + 1),
		JOYSTICK_MAX_ENUM = 0x7FFFFFFF
	};

	enum CursorMode
	{
		CURSOR_MODE_NORMAL = 0,
		CURSOR_MODE_HIDDEN = 1,
		CURSOR_MODE_DISABLED = 2
	};

	enum ShownFlagBits
	{
		SHOWN_HIDDEN_BIT = 0x00000001,
		SHOWN_SHOWN_BIT = 0x00000002,
		SHOWN_MINIMIZED_BIT = 0x00000004,
		SHOWN_MAXIMIZED_BIT = 0x00000008,
		SHOWN_RESTORED_BIT = 0x00000010,
		SHOWN_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
	};

	enum Message
	{
		MESSAGE_YES = 0,
		MESSAGE_YESCANCEL = 1,
		MESSAGE_RETRYCANCEL = 2,
		MESSAGE_YESNO = 3,
		MESSAGE_YESNOCANCEL = 4
	};

	enum MessageResponse
	{
		MESSAGE_RESPONCE_ABORT = 0,
		MESSAGE_RESPONCE_YES = 1,
		MESSAGE_RESPONCE_NO = 2,
		MESSAGE_RESPONCE_CANCEL = 3
	};

	class FL_EXPORT IMonitor
	{
	protected:
		uint32_t m_width;
		uint32_t m_height;
	public:
		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }
	};

	class FL_EXPORT IShell
	{
	protected:
		std::vector<std::shared_ptr<IMonitor>> m_monitors;
		std::vector<const char *> m_extensions;

		bool m_created;

		std::function<void(uint32_t, uint32_t)> m_callbackPosition;
		std::function<void(uint32_t, uint32_t, bool, bool)> m_callbackSize;
		std::function<void(bool)> m_callbackFocus;
		std::function<void()> m_callbackClose;
		std::function<void(float, float, float, float)> m_callbackCursorPosition;
		std::function<void(bool)> m_callbackCursorEnter;
		std::function<void(float, float)> m_callbackCursorScroll;
		std::function<void(MouseButton, bool)> m_callbackMouseButton;
		std::function<void(const char *)> m_callbackChar;
		std::function<void(Key, bool)> m_callbackKey;
		std::function<void(uint32_t, float, float, bool)> m_callbackTouch;
		std::function<void(JoystickPort, std::string, uint32_t, uint32_t, bool)> m_callbackJoystickConnect;
		std::function<void(JoystickPort, uint32_t, bool)> m_callbackJoystickButton;
		std::function<void(JoystickPort, uint32_t, float)> m_callbackJoystickAxis;
	public:
		IShell()
		{
		}

		virtual ~IShell()
		{
		}

		virtual void CreateShell() = 0;

		virtual void CreateSurface(VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) = 0;

		virtual void PollEvents() = 0;

		virtual MessageResponse ShowMessageBox(const std::string &title, const std::string &message, const Message &type) = 0;

		virtual void SetSize(const uint32_t &width, const uint32_t &height) = 0;

		virtual void SetPosition(const uint32_t &x, const uint32_t &y) = 0;

		virtual void SetShown(const uint32_t &shownFlags) = 0;

		virtual void SetResizable(const bool &resizable) = 0;

		virtual void SetTitle(const std::string &title) = 0;

		virtual void SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height) = 0;

		virtual void SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height) = 0;

		virtual void SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen) = 0;

		virtual void SetCursorMode(const CursorMode &mode) = 0;

		virtual void SetCursorPos(const uint32_t &x, const uint32_t &y) = 0;

		std::vector<std::shared_ptr<IMonitor>> GetMonitors() const { return m_monitors; };

		std::vector<const char *> GetExtensions() const { return m_extensions; }

		bool IsCreated() const { return m_created; }

		void SetCallbackPosition(const std::function<void(uint32_t, uint32_t)> &callbackPosition) { m_callbackPosition = callbackPosition; }

		void SetCallbackSize(const std::function<void(uint32_t, uint32_t, bool, bool)> &callbackSize) { m_callbackSize = callbackSize; }

		void SetCallbackFocus(const std::function<void(bool)> &callbackFocus) { m_callbackFocus = callbackFocus; }

		void SetCallbackClose(const std::function<void()> &callbackClose) { m_callbackClose = callbackClose; }

		void SetCallbackCursorPosition(const std::function<void(float, float, float, float)> &callbackCursorPosition) { m_callbackCursorPosition = callbackCursorPosition; }

		void SetCallbackCursorEnter(const std::function<void(bool)> &callbackCursorEnter) { m_callbackCursorEnter = callbackCursorEnter; }

		void SetCallbackCursorScroll(const std::function<void(float, float)> &callbackCursorScroll) { m_callbackCursorScroll = callbackCursorScroll; }

		void SetCallbackMouseButton(const std::function<void(MouseButton, bool)> &callbackMouseButton) { m_callbackMouseButton = callbackMouseButton; }

		void SetCallbackChar(const std::function<void(const char *)> &callbackChar) { m_callbackChar = callbackChar; }

		void SetCallbackKey(const std::function<void(Key, bool)> &callbackKey) { m_callbackKey = callbackKey; }

		void SetCallbackTouch(const std::function<void(uint32_t, float, float, bool)> &callbackTouch) { m_callbackTouch = callbackTouch; }

		void SetCallbackJoystickConnect(const std::function<void(JoystickPort, std::string, uint32_t, uint32_t, bool)> &callbackJoystickConnect) { m_callbackJoystickConnect = callbackJoystickConnect; }

		void SetCallbackJoystickButton(const std::function<void(JoystickPort, uint32_t, bool)> &callbackJoystickButton) { m_callbackJoystickButton = callbackJoystickButton; }

		void SetCallbackJoystickAxis(const std::function<void(JoystickPort, uint32_t, float)> &callbackJoystickAxis) { m_callbackJoystickAxis = callbackJoystickAxis; }
	};
}