#pragma once

#include "IShell.hpp"

namespace acid
{
	class ACID_EXPORT MonitorCocoa :
		public IMonitor
	{
	private:
		uint32_t m_width;
		uint32_t m_height;
	public:
		MonitorCocoa(const uint32_t &width, const uint32_t &height) :
			IMonitor(),
			m_width(width),
			m_height(height)
		{
		}

		~MonitorCocoa()
		{
		}

		uint32_t GetWidth() const override { return m_width; }

		uint32_t GetHeight() const override { return m_height; }
	};

	class ShellCocoa :
		public IShell
	{
	private:

	public:
		ShellCocoa();

		~ShellCocoa();

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
		void LoadVk();

		void CreateDisplay();
	};
}
