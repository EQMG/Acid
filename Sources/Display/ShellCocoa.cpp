#include "ShellCocoa.hpp"

#include <cassert>
#include <cstring>
#include <vulkan/vulkan_macos.h>

namespace acid
{
	ShellCocoa::ShellCocoa() :
		IShell()
	{
		m_extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		m_extensions.emplace_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);

		m_monitors.emplace_back(std::make_shared<MonitorCocoa>(1920, 1080));

		LoadVk();
		CreateDisplay();
	}

	ShellCocoa::~ShellCocoa()
	{
	}

	VkResult ShellCocoa::CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
	{
		*pSurface = VK_NULL_HANDLE;

		VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo;
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.pView = nullptr;

		return vkCreateMacOSSurfaceMVK(instance, &surfaceCreateInfo, pAllocator, pSurface);
	}

	void ShellCocoa::PollEvents()
	{
	}

	MessageResponse ShellCocoa::ShowMessageBox(const std::string &title, const std::string &message, const Message &type)
	{
		return MESSAGE_RESPONSE_NO; // TODO
	}

	void ShellCocoa::SetSize(const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellCocoa::SetPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellCocoa::SetShown(const uint32_t &shownFlags)
	{
		// TODO
	}

	void ShellCocoa::SetResizable(const bool &resizable)
	{
		// TODO
	}

	void ShellCocoa::SetTitle(const std::string &title)
	{
		// TODO
	}

	void ShellCocoa::SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellCocoa::SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellCocoa::SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen)
	{
		// TODO
	}

	void ShellCocoa::SetCursorMode(const CursorMode &mode)
	{
		// TODO
	}

	void ShellCocoa::SetCursorPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellCocoa::LoadVk()
	{

	}

	void ShellCocoa::CreateDisplay()
	{

	}
}
