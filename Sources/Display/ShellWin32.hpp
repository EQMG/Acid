#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <string>
#include "IShell.hpp"

namespace acid
{
	class ACID_EXPORT MonitorWin32 :
		public IMonitor
	{
	private:
		uint32_t m_width;
		uint32_t m_height;
	public:
		MonitorWin32(const uint32_t &width, const uint32_t &height) :
			IMonitor(),
			m_width(width),
			m_height(height)
		{
		}

		~MonitorWin32()
		{
		}

		uint32_t GetWidth() const override { return m_width; }

		uint32_t GetHeight() const override { return m_height; }
	};

	class ACID_EXPORT ShellWin32 :
		public IShell
	{
	private:
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		HMODULE m_hmodule;
		PFN_vkGetInstanceProcAddr m_vkProc;

		uint32_t m_normalWidth, m_normalHeight;
		bool m_fullscreen;

		bool m_cursorTracked;
		bool m_cursorDisabled;
		CursorMode m_cursorMode;
		HCURSOR m_hcursor;

		RAWINPUT *m_rawInput;
		int m_rawInputSize;
	public:
		ShellWin32();

		~ShellWin32();

		VkResult CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface) override;

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

		void SetCursorPosition(const uint32_t &x, const uint32_t &y) override;

	private:
		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void LoadVk();

		void CreateDisplay();

		void CreateInput();

		Key TranslateKey(WPARAM wParam, LPARAM lParam);

		void UpdateCursorImage();

		void UpdateCursorClip(bool cliped);

		HICON CreateIcon(unsigned char *pixels, int width, int height, int xhot, int yhot, VkBool32 icon);
	};
}