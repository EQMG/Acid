#include "Surface.hpp"

#include <cassert>
#include "Renderer/Renderer.hpp"
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

		Renderer::CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice->GetPhysicalDevice(), m_surface, &m_capabilities));

		uint32_t surfaceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->GetPhysicalDevice(), m_surface, &surfaceFormatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->GetPhysicalDevice(), m_surface, &surfaceFormatCount, surfaceFormats.data());

		if ((surfaceFormatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
		{
			m_format.format = VK_FORMAT_B8G8R8A8_UNORM;
			m_format.colorSpace = surfaceFormats[0].colorSpace;
		}
		else
		{
			// iterate over the list of available surface format and
			// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
			bool found_B8G8R8A8_UNORM = false;

			for (auto&& surfaceFormat : surfaceFormats)
			{
				if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
				{
					m_format.format = surfaceFormat.format;
					m_format.colorSpace = surfaceFormat.colorSpace;
					found_B8G8R8A8_UNORM = true;
					break;
				}
			}

			// in case VK_FORMAT_B8G8R8A8_UNORM is not available
			// select the first available color format
			if (!found_B8G8R8A8_UNORM)
			{
				m_format.format = surfaceFormats[0].format;
				m_format.colorSpace = surfaceFormats[0].colorSpace;
			}
		}
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(m_instance->GetInstance(), m_surface, nullptr);
	}
}
