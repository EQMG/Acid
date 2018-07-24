#pragma once

#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <string>
#include "IShell.hpp"

namespace fl
{
	class FL_EXPORT MonitorWin32 :
		public IMonitor
	{
	public:
		MonitorWin32(const uint32_t &width, const uint32_t &height) :
			IMonitor()
		{
			m_width = width;
			m_height = height;
		}

		~MonitorWin32()
		{
		}
	};

	class FL_EXPORT ShellWin32 :
		public IShell
	{
	private:
		// Window.
		HINSTANCE hinstance_;
		HWND hwnd_;
		HMODULE hmodule_;
		PFN_vkGetInstanceProcAddr vkproc_;

		// Fullscreen.
		int nw_, nh_;
		bool fullscreen_;

		// Cursor movement.
		bool cursorTracked_;
		bool cursorDisabled_;
		CursorMode cursorMode_;
		HCURSOR cursor_;

		// Mouse input.
		RAWINPUT *rawInput_;
		int rawInputSize_;
	public:
		ShellWin32();

		~ShellWin32();

		void CreateShell() override;

		void CreateSurface(VkInstance instance, const VkAllocationCallbacks *allocator, VkSurfaceKHR *surface) override;

		void PollEvents() override;

		MessageResponse ShowMessageBox(const std::string &title, const std::string &message, const Message &type) override;

		void SetSize(const uint32_t &width, const uint32_t &height) override;

		void SetPosition(const uint32_t &x, const uint32_t &y) override;

		void SetShown(const uint32_t &shownFlags) override;

		void SetResizable(const bool &resizable) override;

		void SetTitle(const std::string &title) override;

		void SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height) override;

		void SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height) override;

		void SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen) override;

		void SetCursorMode(const CursorMode &mode) override;

		void SetCursorPos(const uint32_t &x, const uint32_t &y) override;

	private:
		Key TranslateKey(WPARAM wParam, LPARAM lParam);

		void UpdateCursorImage();

		void UpdateCursorClip(bool cliped);

		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		HICON CreateIcon(unsigned char *pixels, int width, int height, int xhot, int yhot, VkBool32 icon);

		void LoadVk();

		void CreateDisplay();

		void CreateInput();
	};
}