#include "ShellXcb.hpp"

#include <cassert>
#include <cstring>
#include <vulkan/vulkan_xcb.h>

namespace acid
{
	ShellXcb::ShellXcb() :
		IShell(),
		connection_(nullptr),
		screen_(nullptr),
		window_(nullptr),
		wm_protocols_(nullptr),
		wm_delete_window_(nullptr),
		lib_handle_(nullptr)
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
		xcb_destroy_window(connection_, window_);
		xcb_flush(connection_);
	}

	VkResult ShellXcb::CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
	{
		return VK_SUCCESS;
	}

	void ShellXcb::PollEvents()
	{
		xcb_generic_event_t *ev = xcb_poll_for_event(connection_);

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

	void ShellXcb::InitConnection()
	{
		int scr;

		connection_ = xcb_connect(nullptr, &scr);

		if (!connection_ || xcb_connection_has_error(connection_))
		{
			xcb_disconnect(connection_);
			assert("Failed to connect to the display server.");
		}

		const xcb_setup_t *setup = xcb_get_setup(connection_);
		xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);

		while (scr-- > 0)
		{
			xcb_screen_next(&iter);
		}

		screen_ = iter.data;
	}

	void ShellXcb::LoadVk()
	{
		const char filename[] = "libvulkan.so.1";
		void *handle, *symbol;

#ifdef UNINSTALLED_LOADER
		handle = dlopen(UNINSTALLED_LOADER, RTLD_LAZY);

    if (!handle)
    {
    	handle = dlopen(filename, RTLD_LAZY);
    }
#else
		handle = dlopen(filename, RTLD_LAZY);
#endif

		if (handle) symbol = dlsym(handle, "vkGetInstanceProcAddr");

		if (!handle || !symbol)
		{
			printf("%s\n", dlerror());

			if (handle)
			{
				dlclose(handle);
			}

			assert("Failed to load Vulkan!");
		}

		lib_handle_ = handle;
	}

	void ShellXcb::CreateDisplay()
	{
		window_ = xcb_generate_id(connection_);

		uint32_t value_mask, value_list[32];
		value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
		value_list[0] = screen_->black_pixel;
		value_list[1] = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

		xcb_create_window(connection_, XCB_COPY_FROM_PARENT, window_, screen_->root, 0, 0, 1080, 720, 0,
			XCB_WINDOW_CLASS_INPUT_OUTPUT, screen_->root_visual, value_mask, value_list);

		xcb_intern_atom_cookie_t utf8_string_cookie = intern_atom_cookie(connection_, "UTF8_STRING");
		xcb_intern_atom_cookie_t _net_wm_name_cookie = intern_atom_cookie(connection_, "_NET_WM_NAME");
		xcb_intern_atom_cookie_t wm_protocols_cookie = intern_atom_cookie(connection_, "WM_PROTOCOLS");
		xcb_intern_atom_cookie_t wm_delete_window_cookie = intern_atom_cookie(connection_, "WM_DELETE_WINDOW");

		// set title
		xcb_atom_t utf8_string = intern_atom(connection_, utf8_string_cookie);
		xcb_atom_t _net_wm_name = intern_atom(connection_, _net_wm_name_cookie);
		xcb_change_property(connection_, XCB_PROP_MODE_REPLACE, window_, _net_wm_name, utf8_string, 8, strlen("Acid"),
			"Acid");

		// advertise WM_DELETE_WINDOW
		wm_protocols_ = intern_atom(connection_, wm_protocols_cookie);
		wm_delete_window_ = intern_atom(connection_, wm_delete_window_cookie);
		xcb_change_property(connection_, XCB_PROP_MODE_REPLACE, window_, wm_protocols_, XCB_ATOM_ATOM, 32, 1, &wm_delete_window_);
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

			if (msg->type == wm_protocols_ && msg->data.data32[0] == wm_delete_window_)
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
