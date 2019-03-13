#pragma once

#include "Engine/Engine.hpp"
#include <vulkan/vulkan.h>

namespace acid
{
class Instance;

class ACID_EXPORT PhysicalDevice
{
  public:
	explicit PhysicalDevice(const Instance* instance);

	const VkPhysicalDevice& GetPhysicalDevice() const
	{
		return m_physicalDevice;
	}

	const VkPhysicalDeviceProperties& GetProperties() const
	{
		return m_properties;
	}

	const VkPhysicalDeviceFeatures& GetFeatures() const
	{
		return m_features;
	}

	const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const
	{
		return m_memoryProperties;
	}

	const VkSampleCountFlagBits& GetMsaaSamples() const
	{
		return m_msaaSamples;
	}

  private:
	friend class Renderer;

	VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice>& devices);

	int32_t ScorePhysicalDevice(const VkPhysicalDevice& device);

	VkSampleCountFlagBits GetMaxUsableSampleCount();

	static void LogVulkanDevice(const VkPhysicalDeviceProperties& physicalDeviceProperties, const std::vector<VkExtensionProperties>& extensionProperties);

	const Instance* m_instance;

	VkPhysicalDevice m_physicalDevice;
	VkPhysicalDeviceProperties m_properties;
	VkPhysicalDeviceFeatures m_features;
	VkPhysicalDeviceMemoryProperties m_memoryProperties;
	VkSampleCountFlagBits m_msaaSamples;
};
}
