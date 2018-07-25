#pragma once

#include "IShell.hpp"

namespace fl
{
	class FL_EXPORT MonitorXcb :
		public IMonitor
	{
		MonitorXcb(const uint32_t &width, const uint32_t &height) :
			IMonitor()
		{
			m_width = width;
			m_height = height;
		}

		~MonitorXcb()
		{
		}
	};

	class ShellXcb :
		public IShell
	{
	private:
	public:
		ShellXcb();

		~ShellXcb();

		void CreateShell() override;

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

		void SetCursorPos(const uint32_t &x, const uint32_t &y) override;
	};
}
