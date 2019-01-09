#include "Surface.hpp"

#include <cassert>
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Window.hpp"

namespace acid
{
	Surface::Surface(const Instance *instance, const PhysicalDevice *physicalDevice) :
		m_instance(instance),
		m_physicalDevice(physicalDevice),
		m_surface(VK_NULL_HANDLE),
		m_capabilities({}),
		m_format({})
	{
		// Creates the surface.
		Window::Get()->CreateSurface(m_instance->GetInstance(), nullptr, &m_surface);

		Window::CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice->GetPhysicalDevice(), m_surface, &m_capabilities));

		uint32_t physicalDeviceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->GetPhysicalDevice(), m_surface, &physicalDeviceFormatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> physicalDeviceFormats(physicalDeviceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->GetPhysicalDevice(), m_surface, &physicalDeviceFormatCount, physicalDeviceFormats.data());

		m_format.format = VK_FORMAT_B8G8R8A8_UNORM;
		m_format.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

		if (physicalDeviceFormats[0].format != VK_FORMAT_UNDEFINED)
		{
			m_format = physicalDeviceFormats[0];
		}
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(m_instance->GetInstance(), m_surface, nullptr);
	}
}
