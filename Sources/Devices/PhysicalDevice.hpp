#pragma once

#include <volk.h>
#include <vector>

#include "Export.hpp"

namespace acid {
class Instance;

class ACID_EXPORT PhysicalDevice {
	friend class Graphics;
public:
	explicit PhysicalDevice(const Instance *instance);

	operator const VkPhysicalDevice &() const { return physicalDevice; }

	const VkPhysicalDevice &GetPhysicalDevice() const { return physicalDevice; }
	const VkPhysicalDeviceProperties &GetProperties() const { return properties; }
	const VkPhysicalDeviceFeatures &GetFeatures() const { return features; }
	const VkPhysicalDeviceMemoryProperties &GetMemoryProperties() const { return memoryProperties; }
	const VkSampleCountFlagBits &GetMsaaSamples() const { return msaaSamples; }

private:
	VkPhysicalDevice ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices);
	static uint32_t ScorePhysicalDevice(const VkPhysicalDevice &device);
	VkSampleCountFlagBits GetMaxUsableSampleCount() const;

	static void LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties, const std::vector<VkExtensionProperties> &extensionProperties);

	const Instance *instance;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties properties = {};
	VkPhysicalDeviceFeatures features = {};
	VkPhysicalDeviceMemoryProperties memoryProperties = {};
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
};
}
