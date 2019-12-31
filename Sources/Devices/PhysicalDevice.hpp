#pragma once

#include <vulkan/vulkan.h>
#include "StdAfx.hpp"

namespace acid {
class Instance;

class ACID_EXPORT PhysicalDevice {
	friend class Graphics;
public:
	explicit PhysicalDevice(const Instance *instance);

	operator const VkPhysicalDevice &() const { return m_physicalDevice; }

	const VkPhysicalDevice &GetPhysicalDevice() const { return m_physicalDevice; }
	const VkPhysicalDeviceProperties &GetProperties() const { return m_properties; }
	const VkPhysicalDeviceFeatures &GetFeatures() const { return m_features; }
	const VkPhysicalDeviceMemoryProperties &GetMemoryProperties() const { return m_memoryProperties; }
	VkSampleCountFlagBits GetMsaaSamples() const { return m_msaaSamples; }

private:
	VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices);
	static int32_t ScorePhysicalDevice(const VkPhysicalDevice &device);
	VkSampleCountFlagBits GetMaxUsableSampleCount() const;

	static void LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties, const std::vector<VkExtensionProperties> &extensionProperties);

	const Instance *m_instance;

	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties m_properties = {};
	VkPhysicalDeviceFeatures m_features = {};
	VkPhysicalDeviceMemoryProperties m_memoryProperties = {};
	VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};
}
