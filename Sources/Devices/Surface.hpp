#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"

namespace acid
{
class Instance;
class PhysicalDevice;

class ACID_EXPORT Surface
{
public:
	Surface(const Instance *instance, const PhysicalDevice *physicalDevice);

	~Surface();

	const VkSurfaceKHR &GetSurface() const { return m_surface; }

	const VkSurfaceCapabilitiesKHR &GetCapabilities() const { return m_capabilities; }

	const VkSurfaceFormatKHR &GetFormat() const { return m_format; }

private:
	friend class Renderer;

	const Instance *m_instance;
	const PhysicalDevice *m_physicalDevice;

	VkSurfaceKHR m_surface;
	VkSurfaceCapabilitiesKHR m_capabilities;
	VkSurfaceFormatKHR m_format;
};
}
