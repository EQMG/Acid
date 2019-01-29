#include "PhysicalDevice.hpp"

#include <cassert>
#include <cstring>
#include "Renderer/Renderer.hpp"
#include "Instance.hpp"

namespace acid
{
	static const std::vector<VkSampleCountFlagBits> STAGE_FLAG_BITS =
	{
		VK_SAMPLE_COUNT_64_BIT, VK_SAMPLE_COUNT_32_BIT, VK_SAMPLE_COUNT_16_BIT, VK_SAMPLE_COUNT_8_BIT, VK_SAMPLE_COUNT_4_BIT, VK_SAMPLE_COUNT_2_BIT
	};

	PhysicalDevice::PhysicalDevice(const Instance *instance) :
		m_instance(instance),
		m_physicalDevice(VK_NULL_HANDLE),
		m_properties({}),
		m_features({}),
		m_memoryProperties({}),
		m_msaaSamples(VK_SAMPLE_COUNT_1_BIT)
	{
		uint32_t physicalDeviceCount;
		vkEnumeratePhysicalDevices(m_instance->GetInstance(), &physicalDeviceCount, nullptr);
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(m_instance->GetInstance(), &physicalDeviceCount, physicalDevices.data());

		m_physicalDevice = ChoosePhysicalDevice(physicalDevices);

		if (m_physicalDevice == nullptr)
		{
			assert(false && "Vulkan runtime error, failed to find a suitable gpu!");
		}

		vkGetPhysicalDeviceProperties(m_physicalDevice, &m_properties);
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_features);
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);
		m_msaaSamples = GetMaxUsableSampleCount();

		Log::Out("Selected Physical Device: '%s', %i\n", m_properties.deviceName, m_properties.deviceID);
#if defined(ACID_VERBOSE)
		Log::Out("Max MSAA Samples: %i\n", m_msaaSamples);
#endif
	}

	PhysicalDevice::~PhysicalDevice()
	{
	}

	VkPhysicalDevice PhysicalDevice::ChoosePhysicalDevice(const std::vector<VkPhysicalDevice> &devices)
	{
		// Maps to hold devices and sort by rank.
		std::multimap<int32_t, VkPhysicalDevice> rankedDevices;

		// Iterates through all devices and rate their suitability.
		for (const auto &device : devices)
		{
			int32_t score = ScorePhysicalDevice(device);
			rankedDevices.emplace(score, device);
		}

		// Checks to make sure the best candidate scored higher than 0  rbegin points to last element of ranked devices(highest rated), first is its rating.
		if (rankedDevices.rbegin()->first > 0)
		{
			return rankedDevices.rbegin()->second;
		}

		return nullptr;
	}

	int32_t PhysicalDevice::ScorePhysicalDevice(const VkPhysicalDevice &device)
	{
		int32_t score = 0;

		// Checks if the requested extensions are supported.
		uint32_t extensionPropertyCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

		// Iterates through all extensions requested.
		for (const char *currentExtension : Instance::DeviceExtensions)
		{
			bool extensionFound = false;

			// Checks if the extension is in the available extensions.
			for (const auto &extension : extensionProperties)
			{
				if (strcmp(currentExtension, extension.extensionName) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			// Returns a score of 0 if this device is missing a required extension.
			if (!extensionFound)
			{
				return 0;
			}
		}

		// Obtain the device features and properties of the current device being rateds.
		VkPhysicalDeviceProperties physicalDeviceProperties;
		VkPhysicalDeviceFeatures physicalDeviceFeatures;
		vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(device, &physicalDeviceFeatures);

#if defined(ACID_VERBOSE)
		LogVulkanDevice(physicalDeviceProperties);
#endif

		// Adds a large score boost for discrete GPUs (dedicated graphics cards).
		if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		// Gives a higher score to devices with a higher maximum texture size.
		score += physicalDeviceProperties.limits.maxImageDimension2D;

		return score;
	}

	VkSampleCountFlagBits PhysicalDevice::GetMaxUsableSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(m_physicalDevice, &physicalDeviceProperties);

		VkSampleCountFlags counts = std::min(physicalDeviceProperties.limits.framebufferColorSampleCounts, physicalDeviceProperties.limits.framebufferDepthSampleCounts);

		for (const auto &sampleFlag : STAGE_FLAG_BITS)
		{
			if (counts & sampleFlag)
			{
				return sampleFlag;
			}
		}

		return VK_SAMPLE_COUNT_1_BIT;
	}

	void PhysicalDevice::LogVulkanDevice(const VkPhysicalDeviceProperties &physicalDeviceProperties)
	{
		Log::Out("-- Physical Device: '%s' --\n", physicalDeviceProperties.deviceName);
		Log::Out("ID: %i\n", physicalDeviceProperties.deviceID);

		switch (static_cast<int>(physicalDeviceProperties.deviceType))
		{
			case 1:
				Log::Out("Type: Integrated\n");
				break;
			case 2:
				Log::Out("Type: Discrete\n");
				break;
			case 3:
				Log::Out("Type: Virtual\n");
				break;
			case 4:
				Log::Out("Type: CPU\n");
				break;
			default:
				Log::Out("Type: Other (%x)\n", physicalDeviceProperties.deviceType);
		}

		switch (physicalDeviceProperties.vendorID)
		{
			case 0x8086:
				Log::Out("Vendor: Intel\n");
				break;
			case 0x10DE:
				Log::Out("Vendor: NVIDIA\n");
				break;
			case 0x1002:
				Log::Out("Vendor: AMD\n");
				break;
			default:
				Log::Out("Vendor: Unknown (0x%x)\n", physicalDeviceProperties.vendorID);
		}

		uint32_t supportedVersion[] = {
			VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_MINOR(physicalDeviceProperties.apiVersion),
			VK_VERSION_PATCH(physicalDeviceProperties.apiVersion)
		};
		Log::Out("Supports Version: %i.%i.%i\n", supportedVersion[0], supportedVersion[1], supportedVersion[2]);
		Log::Out("Header Version: %i\n", VK_HEADER_VERSION);
		Log::Out("-- Done --\n");
	}
}
