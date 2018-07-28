#include "ShellCoco.hpp"

#include <cassert>
#include <cstring>
#include <vulkan/vulkan_macos.h>

namespace acid
{
	ShellCoco::ShellCoco() :
		IShell()
	{
		m_extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		m_extensions.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);

		m_monitors.emplace_back(std::make_shared<MonitorCoco>(1920, 1080));

		LoadVk();
		CreateDisplay();
	}

	ShellCoco::~ShellCoco()
	{
	}

	VkResult ShellCoco::CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
	{
		*pSurface = VK_NULL_HANDLE;

		VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo;
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.pView = nullptr;

		return vkCreateMacOSSurfaceMVK(instance, &surfaceCreateInfo, pAllocator, pSurface);
	}

	void ShellCoco::PollEvents()
	{
	}

	MessageResponse ShellCoco::ShowMessageBox(const std::string &title, const std::string &message, const Message &type)
	{
		return MESSAGE_RESPONCE_NO; // TODO
	}

	void ShellCoco::SetSize(const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellCoco::SetPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellCoco::SetShown(const uint32_t &shownFlags)
	{
		// TODO
	}

	void ShellCoco::SetResizable(const bool &resizable)
	{
		// TODO
	}

	void ShellCoco::SetTitle(const std::string &title)
	{
		// TODO
	}

	void ShellCoco::SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellCoco::SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellCoco::SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen)
	{
		// TODO
	}

	void ShellCoco::SetCursorMode(const CursorMode &mode)
	{
		// TODO
	}

	void ShellCoco::SetCursorPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellCoco::LoadVk()
	{

	}

	void ShellCoco::CreateDisplay()
	{

	}
}
