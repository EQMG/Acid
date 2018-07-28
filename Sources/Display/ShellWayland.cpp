#include "ShellWayland.hpp"

#include <cassert>
#include <cstring>
#include <vulkan/vulkan_wayland.h>

namespace acid
{
	ShellWayland::ShellWayland() :
		IShell()
	{
		m_extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		m_extensions.emplace_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);

		m_monitors.emplace_back(std::make_shared<MonitorWayland>(1920, 1080));

		LoadVk();
		CreateDisplay();
	}

	ShellWayland::~ShellWayland()
	{
	}

	VkResult ShellWayland::CreateSurface(VkInstance instance, const VkAllocationCallbacks *pAllocator, VkSurfaceKHR *pSurface)
	{
		*pSurface = VK_NULL_HANDLE;

		VkWaylandSurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.display = nullptr;

		return vkCreateWaylandSurfaceKHR(instance, &surfaceCreateInfo, pAllocator, pSurface);
	}

	void ShellWayland::PollEvents()
	{
	}

	MessageResponse ShellWayland::ShowMessageBox(const std::string &title, const std::string &message, const Message &type)
	{
		return MESSAGE_RESPONCE_NO; // TODO
	}

	void ShellWayland::SetSize(const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellWayland::SetPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellWayland::SetShown(const uint32_t &shownFlags)
	{
		// TODO
	}

	void ShellWayland::SetResizable(const bool &resizable)
	{
		// TODO
	}

	void ShellWayland::SetTitle(const std::string &title)
	{
		// TODO
	}

	void ShellWayland::SetIconImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellWayland::SetCursorImage(unsigned char *data, const uint32_t &width, const uint32_t &height)
	{
		// TODO
	}

	void ShellWayland::SetFullscreen(const std::shared_ptr<IMonitor> &monitor, const bool &fullscreen)
	{
		// TODO
	}

	void ShellWayland::SetCursorMode(const CursorMode &mode)
	{
		// TODO
	}

	void ShellWayland::SetCursorPosition(const uint32_t &x, const uint32_t &y)
	{
		// TODO
	}

	void ShellWayland::LoadVk()
	{

	}

	void ShellWayland::CreateDisplay()
	{

	}
}
