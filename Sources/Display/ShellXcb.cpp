#include "ShellXcb.hpp"

#include <cassert>
#include <cstring>
#include <vulkan/vulkan_xcb.h>

namespace acid
{
	ShellXcb::ShellXcb() :
		IShell(),
		m_connection(nullptr),
		m_screen(nullptr),
		m_window(VK_NULL_HANDLE),
		m_wmProtocols(VK_NULL_HANDLE),
		m_wmDeleteWindow_(VK_NULL_HANDLE),
		m_vkHandle(nullptr)
	{
		m_extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		m_extensions.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);

		m_monitors.emplace_back(std::make_shared<MonitorXcb>(1920, 1080));

		InitConnection();
		LoadVk();
		CreateDisplay();
	}

	ShellXcb::~ShellXcb()
	{
		xcb_destroy_window(m_connection, m_window);
		xcb_flush(m_connection);
	}

	VkResult ShellXcb::CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
	{
		*pSurface = VK_NULL_HANDLE;

		VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.connection = m_connection;
		surfaceCreateInfo.window = m_window;

		return vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, pAllocator, pSurface);
	}

	void ShellXcb::PollEvents()
	{
		xcb_generic_event_t *ev = xcb_poll_for_event(m_connection);

		if (ev != nullptr)
		{
			HandleEvent(ev);
			free(ev);
		}
	}

	MessageResponse ShellXcb::ShowMessageBox(const std::string &title, const std::string &message, const Message &type)
	{
		return MESSAGE_RESPONCE_NO; // TODO
	}

	void ShellXcb::SetSize(const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellXcb::SetPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellXcb::SetShown(const uint32_t &shownFlags)
	{
		// TODO
	}

	void ShellXcb::SetResizable(const bool &resizable)
	{
		// TODO
	}

	void ShellXcb::SetTitle(const std::string &title)
	{
		// TODO
	}

	void ShellXcb::SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellXcb::SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellXcb::SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen)
	{
		// TODO
	}

	void ShellXcb::SetCursorMode(const CursorMode &mode)
	{
		// TODO
	}

	void ShellXcb::SetCursorPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellXcb::HandleEvent(const xcb_generic_event_t *ev)
	{
		switch (ev->response_type & 0x7f)
		{
		case XCB_CONFIGURE_NOTIFY:
		{
			const xcb_configure_notify_event_t *notify = (xcb_configure_notify_event_t *)ev;

			if (m_callbackSize != nullptr)
			{
				m_callbackSize(notify->width, notify->height, 0, 0);
			}

			break;
		}
		case XCB_KEY_PRESS:
		{
			const xcb_key_press_event_t *press = (xcb_key_press_event_t *)ev;

			break;
		}
		case XCB_CLIENT_MESSAGE:
		{
			const xcb_client_message_event_t *msg = (xcb_client_message_event_t *)ev;

			if (msg->type == m_wmProtocols && msg->data.data32[0] == m_wmDeleteWindow_)
			{
				if (m_callbackClose != nullptr)
				{
					m_callbackClose();
				}
			}

			break;
		}
		}
	}

	void ShellXcb::InitConnection()
	{
		int scr;

		m_connection = xcb_connect(nullptr, &scr);

		if (m_connection == nullptr || xcb_connection_has_error(m_connection))
		{
			xcb_disconnect(m_connection);
			assert("Failed to connect to the display server.");
		}

		const xcb_setup_t *setup = xcb_get_setup(m_connection);
		xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

		while (scr-- > 0)
		{
			xcb_screen_next(&iter);
		}

		m_screen = iter.data;
	}

	void ShellXcb::LoadVk()
	{
		const char filename[] = "libvulkan.so.1";
		void *handle, *symbol;

#ifdef UNINSTALLED_LOADER
		handle = dlopen(UNINSTALLED_LOADER, RTLD_LAZY);

        if (handle == nullptr)
        {
        	handle = dlopen(filename, RTLD_LAZY);
        }
#else
		handle = dlopen(filename, RTLD_LAZY);
#endif

		if (handle != nullptr)
		{
		    symbol = dlsym(handle, "vkGetInstanceProcAddr");
		}

		if (handle == nullptr || symbol == nullptr)
		{
			printf("%s\n", dlerror());

			if (handle)
			{
				dlclose(handle);
			}

			assert("Failed to load Vulkan!");
		}

		m_vkHandle = handle;
		m_vkProc = reinterpret_cast<PFN_vkGetInstanceProcAddr>(symbol);
	}

	void ShellXcb::CreateDisplay()
	{
		m_window = xcb_generate_id(m_connection);

		uint32_t value_mask, value_list[32];
		value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
		value_list[0] = m_screen->black_pixel;
		value_list[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

		xcb_create_window(m_connection, XCB_COPY_FROM_PARENT, m_window, m_screen->root, 0, 0, 1080, 720, 0,
			XCB_WINDOW_CLASS_INPUT_OUTPUT, m_screen->root_visual, value_mask, value_list);

		xcb_intern_atom_cookie_t utf8_string_cookie = intern_atom_cookie(m_connection, "UTF8_STRING");
		xcb_intern_atom_cookie_t wm_name_cookie = intern_atom_cookie(m_connection, "WM_NAME");
		xcb_intern_atom_cookie_t wm_protocols_cookie = intern_atom_cookie(m_connection, "WM_PROTOCOLS");
		xcb_intern_atom_cookie_t wm_delete_window_cookie = intern_atom_cookie(m_connection, "WM_DELETE_WINDOW");

		// set title
		xcb_atom_t utf8_string = intern_atom(m_connection, utf8_string_cookie);
		xcb_atom_t wm_name = intern_atom(m_connection, wm_name_cookie);
		xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window, wm_name, utf8_string, 8, strlen("Acid"),
			"Acid");

		// advertise WM_DELETE_WINDOW
		m_wmProtocols = intern_atom(m_connection, wm_protocols_cookie);
		m_wmDeleteWindow_ = intern_atom(m_connection, wm_delete_window_cookie);
		xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window, m_wmProtocols, XCB_ATOM_ATOM, 32, 1, &m_wmDeleteWindow_);

		xcb_map_window(m_connection, m_window);
		xcb_flush(m_connection);
	}

	xcb_intern_atom_cookie_t ShellXcb::intern_atom_cookie(xcb_connection_t *c, const char* s)
	{
		return xcb_intern_atom(c, 0, strlen(s), s);
	}

	xcb_atom_t ShellXcb::intern_atom(xcb_connection_t *c, xcb_intern_atom_cookie_t cookie)
	{
		xcb_atom_t atom = XCB_ATOM_NONE;
		xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(c, cookie, nullptr);

		if (reply)
		{
			atom = reply->atom;
			free(reply);
		}

		return atom;
	}
}
