#include "PhysicalDevice.hpp"

#include <iomanip>

#include "Graphics/Graphics.hpp"
#include "Instance.hpp"

namespace acid {
static const std::vector<VkSampleCountFlagBits> STAGE_FLAG_BITS = {
	VK_SAMPLE_COUNT_64_BIT, VK_SAMPLE_COUNT_32_BIT, VK_SAMPLE_COUNT_16_BIT, VK_SAMPLE_COUNT_8_BIT,
	VK_SAMPLE_COUNT_4_BIT, VK_SAMPLE_COUNT_2_BIT
};

PhysicalDevice::PhysicalDevice(const Instance *instance) :
	instance(instance) {
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(*instance, &physicalDeviceCount, nullptr);
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(*instance, &physicalDeviceCount, physicalDevices.data());

	// TODO: Allow user to configure graphics preference.
	physicalDevice = ChoosePhysicalDevice(physicalDevices);
	if (!physicalDevice)
		throw std::runtime_error("Vulkan runtime error, failed to find a suitable GPU");

	vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	vkGetPhysicalDeviceFeatures(physicalDevice, &features);
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
	msaaSamples = GetMaxUsableSampleCount();

#ifdef ACID_DEBUG
	Log::Out("Selected Physical Device: ", properties.deviceID, " ", std::quoted(properties.deviceName), '\n');
#endif
}

VkPhysicalDevice PhysicalDevice::ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices) {
	// Maps to hold devices and sort by rank.
	std::multimap<uint32_t, VkPhysicalDevice> rankedDevices;
	auto where = rankedDevices.end();

	// Iterates through all devices and rate their suitability.
	for (const auto &device : devices)
		where = rankedDevices.insert(where, {ScorePhysicalDevice(device), device});

	// Checks to make sure the best candidate scored higher than 0  rbegin points to last element of ranked devices(highest rated), first is its rating.
	if (rankedDevices.rbegin()->first > 0)
		return rankedDevices.rbegin()->second;

	return nullptr;
}

uint32_t PhysicalDevice::ScorePhysicalDevice(const VkPhysicalDevice &device) {
	uint32_t score = 0;

	// Checks if the requested extensions are supported.
	uint32_t extensionPropertyCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
	std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

	// Iterates through all extensions requested.
	for (const char *currentExtension : LogicalDevice::DeviceExtensions) {
		bool extensionFound = false;

		// Checks if the extension is in the available extensions.
		for (const auto &extension : extensionProperties) {
			if (strcmp(currentExtension, extension.extensionName) == 0) {
				extensionFound = true;
				break;
			}
		}

		// Returns a score of 0 if this device is missing a required extension.
		if (!extensionFound)
			return 0;
	}

	// Obtain the device features and properties of the current device being rateds.
	VkPhysicalDeviceProperties physicalDeviceProperties;
	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

#ifdef ACID_DEBUG
	LogVulkanDevice(physicalDeviceProperties, extensionProperties);
#endif

	// Adds a large score boost for discrete GPUs (dedicated graphics cards).
	if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	// Gives a higher score to devices with a higher maximum texture size.
	score += physicalDeviceProperties.limits.maxImageDimension2D;
	return score;
}

VkSampleCountFlagBits PhysicalDevice::GetMaxUsableSampleCount() const {
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	auto counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);

	for (const auto &sampleFlag : STAGE_FLAG_BITS) {
		if (counts & sampleFlag)
			return sampleFlag;
	}

	return VK_SAMPLE_COUNT_1_BIT;
}

void PhysicalDevice::LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties, const std::vector<VkExtensionProperties> &extensionProperties) {
	std::stringstream ss;
	switch (static_cast<int32_t>(physicalDeviceProperties.deviceType)) {
	case 1:
		ss << "Integrated";
		break;
	case 2:
		ss << "Discrete";
		break;
	case 3:
		ss << "Virtual";
		break;
	case 4:
		ss << "CPU";
		break;
	default:
		ss << "Other " << physicalDeviceProperties.deviceType;
	}

	ss << " Physical Device: " << physicalDeviceProperties.deviceID;
	switch (physicalDeviceProperties.vendorID) {
	case 0x8086:
		ss << " \"Intel\"";
		break;
	case 0x10DE:
		ss << " \"Nvidia\"";
		break;
	case 0x1002:
		ss << " \"AMD\"";
		break;
	default:
		ss << " \"" << physicalDeviceProperties.vendorID << '\"';
	}

	ss << " " << std::quoted(physicalDeviceProperties.deviceName) << '\n';

	uint32_t supportedVersion[3] = {
		VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion),
		VK_VERSION_MINOR(physicalDeviceProperties.apiVersion),
		VK_VERSION_PATCH(physicalDeviceProperties.apiVersion)
	};
	ss << "API Version: " << supportedVersion[0] << "." << supportedVersion[1] << "." << supportedVersion[2] << '\n';

	ss << "Extensions: ";
	for (const auto &extension : extensionProperties)
		ss << extension.extensionName << ", ";

	ss << "\n\n";
	Log::Out(ss.str());
}
}
