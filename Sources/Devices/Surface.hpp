#pragma once

#include <vulkan/vulkan.h>
#include "StdAfx.hpp"

namespace acid
{
class Instance;
class PhysicalDevice;

class ACID_EXPORT Surface
{
public:
	Surface(const Instance *instance, const PhysicalDevice *physicalDevice);

	~Surface();

	operator const VkSurfaceKHR &() const { return m_surface; }

	const VkSurfaceKHR &GetSurface() const { return m_surface; }

	const VkSurfaceCapabilitiesKHR &GetCapabilities() const { return m_capabilities; }

	const VkSurfaceFormatKHR &GetFormat() const { return m_format; }

private:
	friend class Graphics;

	const Instance *m_instance;
	const PhysicalDevice *m_physicalDevice;

	VkSurfaceKHR m_surface{VK_NULL_HANDLE};
	VkSurfaceCapabilitiesKHR m_capabilities{};
	VkSurfaceFormatKHR m_format{};
};
}
