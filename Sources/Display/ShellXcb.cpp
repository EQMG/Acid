#include "ShellXcb.hpp"

namespace acid
{
	ShellXcb::ShellXcb()
	{
	}

	ShellXcb::~ShellXcb()
	{
	}

	void ShellXcb::CreateShell()
	{
	}

	VkResult ShellXcb::CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
	{
		return VK_SUCCESS;
	}

	void ShellXcb::PollEvents()
	{
	}

	MessageResponse ShellXcb::ShowMessageBox(const std::string &title, const std::string &message, const Message &type)
	{
		return MESSAGE_RESPONCE_NO;
	}

	void ShellXcb::SetSize(const uint32_t &width, const uint32_t &height)
	{
	}

	void ShellXcb::SetPosition(const uint32_t &x, const uint32_t &y)
	{
	}

	void ShellXcb::SetShown(const uint32_t &shownFlags)
	{
	}

	void ShellXcb::SetResizable(const bool &resizable)
	{
	}

	void ShellXcb::SetTitle(const std::string &title)
	{
	}

	void ShellXcb::SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
	}

	void ShellXcb::SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
	}

	void ShellXcb::SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen)
	{
	}

	void ShellXcb::SetCursorMode(const CursorMode &mode)
	{
	}

	void ShellXcb::SetCursorPos(const uint32_t &x, const uint32_t &y)
	{
	}
}
