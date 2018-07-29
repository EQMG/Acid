#include "ShellWin32.hpp"

#include <cassert>
#include <vulkan/vulkan_win32.h>

namespace acid
{
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif

#define STYLE_NORMAL (WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define STYLE_FULLSCREEN (WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_SYSMENU | WS_POPUP)

	static const int WIN32_TO_KEY[512] = {
		0, KEY_ESCAPE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL, KEY_BACKSPACE, KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
		KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET, KEY_ENTER, KEY_LEFT_CONTROL, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE, KEY_GRAVE_ACCENT, KEY_LEFT_SHIFT, KEY_BACKSLASH,
		KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_RIGHT_SHIFT, KEY_PAD_MULTIPLY, KEY_LEFT_ALT, KEY_SPACE, KEY_CAPS_LOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_PAUSE, KEY_SCROLL_LOCK, KEY_PAD_7, KEY_PAD_8, KEY_PAD_9, KEY_PAD_SUBTRACT, KEY_PAD_4, KEY_PAD_5, KEY_PAD_6, KEY_PAD_ADD, KEY_PAD_1, KEY_PAD_2, KEY_PAD_3, KEY_PAD_0,
		KEY_PAD_DECIMAL, 0, 0, KEY_WORLD_2, KEY_F11, KEY_F12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, 0, 0, 0, 0, 0, 0, 0,
		KEY_F24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_PAD_ENTER, KEY_RIGHT_CONTROL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_PAD_DIVIDE, 0,
		KEY_PRINT_SCREEN, KEY_RIGHT_ALT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KEY_NUM_LOCK, KEY_PAUSE, KEY_HOME, KEY_UP, KEY_PAGE_UP, 0, KEY_LEFT, 0, KEY_RIGHT, 0, KEY_END, KEY_DOWN, KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE,
		0, 0, 0, 0, 0, 0, 0, KEY_LEFT_SUPER, KEY_RIGHT_SUPER, KEY_MENU, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	ShellWin32::ShellWin32() :
		IShell(),
		m_hinstance(VK_NULL_HANDLE),
		m_hwnd(VK_NULL_HANDLE),
		m_hmodule(VK_NULL_HANDLE),
		m_vkProc(VK_NULL_HANDLE),
		m_normalWidth(0),
		m_normalHeight(0),
		m_fullscreen(false),
		m_cursorTracked(false),
		m_cursorDisabled(false),
		m_cursorMode(CURSOR_MODE_NORMAL),
		m_hcursor(VK_NULL_HANDLE),
		m_rawInput(VK_NULL_HANDLE),
		m_rawInputSize(0)
	{
		m_extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		m_extensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

		m_monitors.emplace_back(std::make_shared<MonitorWin32>(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));

		LoadVk();
		CreateDisplay();
		CreateInput();
	}

	ShellWin32::~ShellWin32()
	{
		CloseWindow(m_hwnd);
		FreeLibrary(m_hmodule);
	}

	VkResult ShellWin32::CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
	{
		*pSurface = VK_NULL_HANDLE;

		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = m_hinstance;
		surfaceCreateInfo.hwnd = m_hwnd;

		return vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, pAllocator, pSurface);
	}

	void ShellWin32::PollEvents()
	{
		RECT area;
		GetClientRect(m_hwnd, &area);

		if (m_cursorDisabled)
		{
			SetCursorPos(area.right / 2, area.bottom / 2);
		}

		MSG msg;

		while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				if (m_callbackClose != nullptr)
				{
					m_callbackClose();
				}

				return;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	MessageResponse ShellWin32::ShowMessageBox(const std::string &title, const std::string &message, const Message &type)
	{
		UINT uType = 0;

		if (type == MESSAGE_YES)
		{
			uType = MB_OK;
		}
		else if (type == MESSAGE_YESCANCEL)
		{
			uType = MB_OKCANCEL;
		}
		else if (type == MESSAGE_RETRYCANCEL)
		{
			uType = MB_RETRYCANCEL;
		}
		else if (type == MESSAGE_YESNO)
		{
			uType = MB_YESNO;
		}
		else if (type == MESSAGE_YESNOCANCEL)
		{
			uType = MB_YESNOCANCEL;
		}

		int response = MessageBox(m_hwnd, message.c_str(), title.c_str(), uType);

		if (response != 0)
		{
			if (response == IDABORT)
			{
				return MESSAGE_RESPONSE_ABORT;
			}
			else if (response == IDCANCEL)
			{
				return MESSAGE_RESPONSE_CANCEL;
			}
			else if (response == IDOK || response == IDYES)
			{
				return MESSAGE_RESPONSE_YES;
			}
			else if (response == IDNO)
			{
				return MESSAGE_RESPONSE_NO;
			}
		}

		return MESSAGE_RESPONSE_NO;
	}

	void ShellWin32::SetSize(const uint32_t &width, const uint32_t &height)
	{
		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
	}

	void ShellWin32::SetPosition(const uint32_t &x, const uint32_t &y)
	{
		SetWindowPos(m_hwnd, HWND_TOP, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
	}

	void ShellWin32::SetShown(const uint32_t &shownFlags)
	{
		int nCmdShow = 0;

		if (shownFlags & SHOWN_HIDDEN_BIT)
		{
			nCmdShow |= SW_HIDE;
		}
		if (shownFlags & SHOWN_SHOWN_BIT)
		{
			nCmdShow |= SW_SHOW;
		}
		if (shownFlags & SHOWN_MINIMIZED_BIT)
		{
			nCmdShow |= SW_MINIMIZE;
		}
		if (shownFlags & SHOWN_MAXIMIZED_BIT)
		{
			nCmdShow |= SW_MAXIMIZE;
		}
		if (shownFlags & SHOWN_RESTORED_BIT)
		{
			nCmdShow |= SW_RESTORE;
		}

		ShowWindow(m_hwnd, nCmdShow);
	}

	void ShellWin32::SetResizable(const bool &resizable)
	{
		// TODO: Resizable
	}

	void ShellWin32::SetTitle(const std::string &title)
	{
		SetWindowText(m_hwnd, title.c_str());
	}

	void ShellWin32::SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		HICON bigIcon = CreateIcon(data, width, height, 0, 0, true);

		if (bigIcon == nullptr)
		{
			return;
		}

		SendMessage(m_hwnd, WM_SETICON, ICON_BIG, (LPARAM) bigIcon);
	}

	void ShellWin32::SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		HICON icon = CreateIcon(data, width, height, 0, 0, false);

		if (icon == nullptr)
		{
			return;
		}

		m_hcursor = (HCURSOR) icon;
	}

	void ShellWin32::SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen)
	{
		int displayWidth = GetSystemMetrics(SM_CXSCREEN);
		int displayHeight = GetSystemMetrics(SM_CYSCREEN);

		if (fullscreen)
		{
			RECT windowRect;
			GetClientRect(m_hwnd, &windowRect);
			m_normalWidth = windowRect.right - windowRect.left;
			m_normalHeight = windowRect.bottom - windowRect.top;

			SetWindowLongPtr(m_hwnd, GWL_STYLE, STYLE_FULLSCREEN);
			MoveWindow(m_hwnd, 0, 0, displayWidth, displayHeight, true);
		}
		else
		{
			SetWindowLongPtr(m_hwnd, GWL_STYLE, STYLE_NORMAL);
			MoveWindow(m_hwnd, (displayWidth - m_normalWidth) / 2, (displayHeight - m_normalHeight) / 2, m_normalWidth, m_normalHeight, true);
		}

		m_fullscreen = fullscreen;
	}

	void ShellWin32::SetCursorMode(const CursorMode &mode)
	{
		m_cursorMode = mode;

		if (mode == CURSOR_MODE_DISABLED)
		{
			const RAWINPUTDEVICE rid = {HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_MOUSE, 0, m_hwnd};

			m_cursorDisabled = true;
			UpdateCursorImage();
			UpdateCursorClip(true);

			if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			{
				fprintf(stderr, "Win32: Failed to register raw input device\n");
			}
		}
		else
		{
			const RAWINPUTDEVICE rid = {HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_MOUSE, RIDEV_REMOVE, nullptr};

			m_cursorDisabled = false;
			UpdateCursorClip(false);
			UpdateCursorImage();

			if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			{
				fprintf(stderr, "Win32: Failed to remove raw input device\n");
			}
		}
	}

	void ShellWin32::SetCursorPosition(const uint32_t &x, const uint32_t &y)
	{
		SetCursorPos(x, y);
	}

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		ShellWin32 *shell = (ShellWin32 *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (shell == nullptr)
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		switch (uMsg)
		{
		case WM_DISPLAYCHANGE:
		{
			// TODO: Update monitor list
			break;
		}
		case WM_MOVE:
		{
			UINT x = LOWORD(lParam);
			UINT y = HIWORD(lParam);

			if (shell->m_cursorDisabled)
			{
				shell->UpdateCursorClip(shell);
			}

			if (shell->m_callbackPosition != nullptr)
			{
				shell->m_callbackPosition(x, y);
			}

			break;
		}
		case WM_SIZE:
		{
			const int iconified = wParam == SIZE_MINIMIZED;
			UINT w = LOWORD(lParam);
			UINT h = HIWORD(lParam);

			if (shell->m_cursorDisabled)
			{
				shell->UpdateCursorClip(shell);
			}

			if (shell->m_callbackSize != nullptr)
			{
				shell->m_callbackSize(w, h, iconified, shell->m_fullscreen);
			}

			break;
		}
		case WM_SETFOCUS:
		{
			if (shell->m_callbackFocus != nullptr)
			{
				shell->m_callbackFocus(true);
			}

			break;
		}
		case WM_KILLFOCUS:
		{
			if (shell->m_callbackFocus != nullptr)
			{
				shell->m_callbackFocus(false);
			}

			break;
		}
		case WM_ERASEBKGND:
		{
			return true;
		}
		case WM_PAINT:
		{
			ValidateRect(shell->m_hwnd, nullptr);
			break;
		}
		case WM_DESTROY:
		{
			break;
		}
		case WM_CLOSE:
		{
			if (shell->m_callbackClose != nullptr)
			{
				shell->m_callbackClose();
			}

			break;
		}
		case WM_SETCURSOR:
		{
			if (LOWORD(lParam) == HTCLIENT)
			{
				shell->UpdateCursorImage();

				return true;
			}

			break;
		}
		case WM_INPUT:
		{
			UINT dwSize;
			GetRawInputData((HRAWINPUT) lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

			if (dwSize > (UINT) shell->m_rawInputSize)
			{
				free(shell->m_rawInput);
				shell->m_rawInput = (RAWINPUT *) calloc(dwSize, 1);
				shell->m_rawInputSize = dwSize;
			}

			UINT size = shell->m_rawInputSize;

			if (GetRawInputData((HRAWINPUT) lParam, RID_INPUT, shell->m_rawInput, &size,
				sizeof(RAWINPUTHEADER)) == (UINT) -1)
			{
				fprintf(stderr, "Win32: Failed to retrieve raw input data\n");
				break;
			}

			RAWINPUT *raw = shell->m_rawInput;

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				// WIN32_TO_WSI_KEY[raw->data.keyboard.VKey & 0x1FF]
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				POINT ptCursor;
				GetCursorPos(&ptCursor);
				ScreenToClient(shell->m_hwnd, &ptCursor);
				RECT windowRect;
				GetClientRect(shell->m_hwnd, &windowRect);

				if (shell->m_callbackCursorPosition != nullptr)
				{
					float width = (float) (windowRect.right - windowRect.left);
					float height = (float) (windowRect.bottom - windowRect.top);

					shell->m_callbackCursorPosition((float) ptCursor.x / width, (float) ptCursor.y / height,
						-(float) raw->data.mouse.lLastX / width, -(float) raw->data.mouse.lLastY / height);
				}
			}

			break;
		}
		case WM_MOUSEMOVE:
		{
			//	int x = GET_X_LPARAM(lParam);
			// 	int y = GET_Y_LPARAM(lParam);

			if (!shell->m_cursorTracked)
			{
				if (shell->m_callbackCursorEnter != nullptr)
				{
					shell->m_callbackCursorEnter(true);
				}

				TRACKMOUSEEVENT tme;
				ZeroMemory(&tme, sizeof(tme));
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = shell->m_hwnd;
				TrackMouseEvent(&tme);

				shell->m_cursorTracked = 1;
			}

			break;
		}
		case WM_MOUSELEAVE:
		{
			if (shell->m_callbackCursorEnter != nullptr)
			{
				shell->m_callbackCursorEnter(false);
			}

			shell->m_cursorTracked = 0;

			break;
		}
		case WM_MOUSEHWHEEL:
		{
			if (shell->m_callbackCursorScroll != nullptr)
			{
				shell->m_callbackCursorScroll(-(SHORT) HIWORD(wParam) / (float) WHEEL_DELTA, 0.0f);
			}

			break;
		}
		case WM_MOUSEWHEEL:
		{
			if (shell->m_callbackCursorScroll != nullptr)
			{
				shell->m_callbackCursorScroll(0.0f, (SHORT) HIWORD(wParam) / (float) WHEEL_DELTA);
			}

			break;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			MouseButton button;
			bool action;

			if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
			{
				button = MOUSE_BUTTON_LEFT;
			}
			else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP)
			{
				button = MOUSE_BUTTON_RIGHT;
			}
			else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
			{
				button = MOUSE_BUTTON_MIDDLE;
			}
			else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				button = MOUSE_BUTTON_4;
			}
			else
			{
				button = MOUSE_BUTTON_5;
			}

			if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
				uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN)
			{
				action = true;
			}
			else
			{
				action = false;
			}

			if (shell->m_callbackMouseButton != nullptr)
			{
				shell->m_callbackMouseButton(button, action);
			}

			break;
		}
		case WM_CHAR:
		case WM_SYSCHAR:
		case WM_UNICHAR:
		{
			if (uMsg == WM_UNICHAR && wParam == UNICODE_NOCHAR)
			{
				return true;
			}

			static char charBuf[4];
			strncpy(charBuf, (const char *) &wParam, 4);

			if (shell->m_callbackChar != nullptr)
			{
				shell->m_callbackChar(charBuf);
			}

			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			Key key = shell->TranslateKey(wParam, lParam);
			bool action = ((lParam >> 31) & 1) ? false : true;

			if (key == KEY_UNKNOWN)
			{
				break;
			}

			if (shell->m_callbackKey != nullptr)
			{
				if (!action && wParam == VK_SHIFT)
				{
					// Release both Shift keys on Shift up event
					shell->m_callbackKey(KEY_LEFT_SHIFT, action);
					shell->m_callbackKey(KEY_RIGHT_SHIFT, action);
				}
				else if (wParam == VK_SNAPSHOT)
				{
					// Key down is not reported for the Print Screen key
					shell->m_callbackKey(key, true);
					shell->m_callbackKey(key, false);
				}
				else
				{
					shell->m_callbackKey(key, action);
				}
			}

			break;
		}
#ifdef ENABLE_MULTITOUCH
			case WM_POINTERUPDATE:
			case WM_POINTERDOWN:
			case WM_POINTERUP:
			{
				uint32_t x = GET_X_LPARAM(lParam);
				uint32_t y = GET_Y_LPARAM(lParam);
				bool action;
				POINTER_INFO pointerInfo;

				if (GetPointerInfo(GET_POINTERID_WPARAM(wParam), &pointerInfo))
				{
					uint32_t id = pointerInfo.pointerId;
					POINT pt = pointerInfo.ptPixelLocation;
					ScreenToClient(shell->hwnd_, &pt);
					RECT windowRect;
					GetClientRect(shell->hwnd_, &windowRect);

					float width = (float)(windowRect.right - windowRect.left);
					float height = (float)(windowRect.bottom - windowRect.top);

					switch (uMsg)
					{
						case WM_POINTERUPDATE:
						case WM_POINTERDOWN:
						{
							action = true;
							break;
						}
						case WM_POINTERUP:
						{
							action = false;
							break;
						}
					}

					if (shell->m_callbackTouch != nullptr)
					{
						shell->m_callbackTouch(id, (float)x / width, (float)y / height, action);
					}
				}
				break;
			}
#endif
			// TODO: Joystick connect, button, axis
		}

		return DefWindowProc(shell->m_hwnd, uMsg, wParam, lParam);
	}

	void ShellWin32::LoadVk()
	{
		const char filename[] = "vulkan-1.dll";
		m_hmodule = LoadLibrary(filename);
		m_vkProc = nullptr;

		if (m_hmodule)
		{
			m_vkProc = reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetProcAddress(m_hmodule, "vkGetInstanceProcAddr"));
		}

		if (!m_hmodule || !m_vkProc)
		{
			assert("Failed to load Vulkan!");
		}
	}

	void ShellWin32::CreateDisplay()
	{
		m_hinstance = GetModuleHandle(nullptr);

		WNDCLASSEX window_class = {};
		window_class.cbSize = sizeof(WNDCLASSEX);
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = WindowProc;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = m_hinstance;
		window_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		window_class.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
		window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		window_class.lpszMenuName = nullptr;
		window_class.lpszClassName = "ShellWin32";

		ATOM atom = RegisterClassEx(&window_class);
		assert(atom && "Failed to register the window class.");

		RECT window_rect = {0, 0, 1080, 720};
		AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);

		m_hwnd = CreateWindowEx(0, "ShellWin32", "Acid", STYLE_NORMAL, 0, 0,
			window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, m_hinstance, nullptr);
		assert(m_hwnd && "Failed to create a window.");

		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}

	void ShellWin32::CreateInput()
	{
		RAWINPUTDEVICE Rid[2];

		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x02;
		Rid[0].dwFlags = 0;
		Rid[0].hwndTarget = m_hwnd;

		Rid[1].usUsagePage = 0x01;
		Rid[1].usUsage = 0x06;
		Rid[1].dwFlags = 0;
		Rid[1].hwndTarget = m_hwnd;

		if (!RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])))
		{
			fprintf(stderr, "Win32: Failed to register raw input device\n");
		}
	}

	Key ShellWin32::TranslateKey(WPARAM wParam, LPARAM lParam)
	{
		// The Ctrl keys require special handling
		if (wParam == VK_CONTROL)
		{
			MSG next;
			DWORD time;

			// Right side keys have the extended key bit set
			if (lParam & 0x01000000)
			{
				return KEY_RIGHT_CONTROL;
			}

			// If Right Alt, ignore this (synthetic) Left Ctrl message
			time = GetMessageTime();

			if (PeekMessageW(&next, nullptr, 0, 0, PM_NOREMOVE))
			{
				if (next.message == WM_KEYDOWN ||
					next.message == WM_SYSKEYDOWN ||
					next.message == WM_KEYUP ||
					next.message == WM_SYSKEYUP)
				{
					if (next.wParam == VK_MENU &&
						(next.lParam & 0x01000000) &&
						next.time == time)
					{
						// Next message is Right Alt down so discard this
						return KEY_UNKNOWN;
					}
				}
			}

			return KEY_LEFT_CONTROL;
		}

		if (wParam == VK_PROCESSKEY)
		{
			// IME notifies that keys have been filtered by setting the virtual key-code to VK_PROCESSKEY
			return KEY_UNKNOWN;
		}

		return (Key) WIN32_TO_KEY[HIWORD(lParam) & 0x1FF];
	}

	void ShellWin32::UpdateCursorImage()
	{
		if (m_cursorMode == CURSOR_MODE_NORMAL)
		{
			if (m_hcursor != nullptr)
			{
				SetCursor(m_hcursor);
			}
			else
			{
				SetCursor(LoadCursorW(nullptr, (LPCWSTR) IDC_ARROW));
			}
		}
		else
		{
			SetCursor(nullptr);
		}
	}

	void ShellWin32::UpdateCursorClip(bool cliped)
	{
		if (cliped)
		{
			RECT clipRect;
			GetClientRect(m_hwnd, &clipRect);
			ClientToScreen(m_hwnd, (POINT *) &clipRect.left);
			ClientToScreen(m_hwnd, (POINT *) &clipRect.right);
			ClipCursor(&clipRect);
		}
		else
		{
			ClipCursor(nullptr);
		}
	}

	HICON ShellWin32::CreateIcon(unsigned char *pixels, int width, int height, int xhot, int yhot, VkBool32 icon)
	{
		if (pixels == nullptr)
		{
			return nullptr;
		}

		int i;
		HDC dc;
		HICON handle;
		HBITMAP color, mask;
		BITMAPV5HEADER bi;
		ICONINFO ii;
		unsigned char *target = nullptr;
		unsigned char *source = pixels;

		ZeroMemory(&bi, sizeof(bi));
		bi.bV5Size = sizeof(bi);
		bi.bV5Width = width;
		bi.bV5Height = -height;
		bi.bV5Planes = 1;
		bi.bV5BitCount = 32;
		bi.bV5Compression = BI_BITFIELDS;
		bi.bV5RedMask = 0x00ff0000;
		bi.bV5GreenMask = 0x0000ff00;
		bi.bV5BlueMask = 0x000000ff;
		bi.bV5AlphaMask = 0xff000000;

		dc = GetDC(nullptr);
		color = CreateDIBSection(dc, (BITMAPINFO *) &bi, DIB_RGB_COLORS,
			(void **) &target, nullptr, (DWORD) 0);
		ReleaseDC(nullptr, dc);

		if (!color)
		{
			fprintf(stderr, "Win32: Failed to create RGBA bitmap\n");
			return nullptr;
		}

		mask = CreateBitmap(width, height, 1, 1, nullptr);

		if (!mask)
		{
			fprintf(stderr, "Win32: Failed to create mask bitmap\n");
			DeleteObject(color);
			return nullptr;
		}

		for (i = 0; i < width * height; i++)
		{
			target[0] = source[2];
			target[1] = source[1];
			target[2] = source[0];
			target[3] = source[3];
			target += 4;
			source += 4;
		}

		ZeroMemory(&ii, sizeof(ii));
		ii.fIcon = icon;
		ii.xHotspot = xhot;
		ii.yHotspot = yhot;
		ii.hbmMask = mask;
		ii.hbmColor = color;

		handle = CreateIconIndirect(&ii);

		DeleteObject(color);
		DeleteObject(mask);

		if (!handle)
		{
			if (icon)
			{
				fprintf(stderr, "Win32: Failed to create icon\n");
			}
			else
			{
				fprintf(stderr, "Win32: Failed to create cursor\n");
			}
		}

		return handle;
	}
}
