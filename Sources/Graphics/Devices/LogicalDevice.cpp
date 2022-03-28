#include "LogicalDevice.hpp"

#include "Graphics/Graphics.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"

namespace acid {
const std::vector<const char *> LogicalDevice::DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME}; // VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME

LogicalDevice::LogicalDevice(const Instance &instance, const PhysicalDevice &physicalDevice) :
	instance(instance),
	physicalDevice(physicalDevice) {
	CreateQueueIndices();
	CreateLogicalDevice();
}

LogicalDevice::~LogicalDevice() {
	Graphics::CheckVk(vkDeviceWaitIdle(logicalDevice));

	vkDestroyDevice(logicalDevice, nullptr);
}

void LogicalDevice::CreateQueueIndices() {
	uint32_t deviceQueueFamilyPropertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyPropertyCount, nullptr);
	std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

	std::optional<uint32_t> graphicsFamily, presentFamily, computeFamily, transferFamily;

	for (uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++) {
		// Check for graphics support.
		if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamily = i;
			this->graphicsFamily = i;
			supportedQueues |= VK_QUEUE_GRAPHICS_BIT;
		}

		// Check for presentation support.
		//VkBool32 presentSupport;
		//vkGetPhysicalDeviceSurfaceSupportKHR(*physicalDevice, i, *surface, &presentSupport);

		if (deviceQueueFamilyProperties[i].queueCount > 0 /*&& presentSupport*/) {
			presentFamily = i;
			this->presentFamily = i;
		}

		// Check for compute support.
		if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			computeFamily = i;
			this->computeFamily = i;
			supportedQueues |= VK_QUEUE_COMPUTE_BIT;
		}

		// Check for transfer support.
		if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			transferFamily = i;
			this->transferFamily = i;
			supportedQueues |= VK_QUEUE_TRANSFER_BIT;
		}

		if (graphicsFamily && presentFamily && computeFamily && transferFamily) {
			break;
		}
	}

	if (!graphicsFamily)
		throw std::runtime_error("Failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT");
}

void LogicalDevice::CreateLogicalDevice() {
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriorities[1] = {0.0f};

	if (supportedQueues & VK_QUEUE_GRAPHICS_BIT) {
		VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
		graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueCreateInfo.queueFamilyIndex = graphicsFamily;
		graphicsQueueCreateInfo.queueCount = 1;
		graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.emplace_back(graphicsQueueCreateInfo);
	} else {
		graphicsFamily = 0; // VK_NULL_HANDLE;
	}

	if (supportedQueues & VK_QUEUE_COMPUTE_BIT && computeFamily != graphicsFamily) {
		VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
		computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		computeQueueCreateInfo.queueFamilyIndex = computeFamily;
		computeQueueCreateInfo.queueCount = 1;
		computeQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.emplace_back(computeQueueCreateInfo);
	} else {
		computeFamily = graphicsFamily;
	}

	if (supportedQueues & VK_QUEUE_TRANSFER_BIT && transferFamily != graphicsFamily && transferFamily != computeFamily) {
		VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
		transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transferQueueCreateInfo.queueFamilyIndex = transferFamily;
		transferQueueCreateInfo.queueCount = 1;
		transferQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.emplace_back(transferQueueCreateInfo);
	} else {
		transferFamily = graphicsFamily;
	}

	auto physicalDeviceFeatures = physicalDevice.GetFeatures();
	VkPhysicalDeviceFeatures enabledFeatures = {};

	// Enable sample rate shading filtering if supported.
	if (physicalDeviceFeatures.sampleRateShading)
		enabledFeatures.sampleRateShading = VK_TRUE;

	// Fill mode non solid is required for wireframe display.
	if (physicalDeviceFeatures.fillModeNonSolid) {
		enabledFeatures.fillModeNonSolid = VK_TRUE;

		// Wide lines must be present for line width > 1.0f.
		if (physicalDeviceFeatures.wideLines)
			enabledFeatures.wideLines = VK_TRUE;
	} else {
		Log::Warning("Selected GPU does not support wireframe pipelines!\n");
	}

	if (physicalDeviceFeatures.samplerAnisotropy)
		enabledFeatures.samplerAnisotropy = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support sampler anisotropy!\n");

	if (physicalDeviceFeatures.textureCompressionBC)
		enabledFeatures.textureCompressionBC = VK_TRUE;
	else if (physicalDeviceFeatures.textureCompressionASTC_LDR)
		enabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
	else if (physicalDeviceFeatures.textureCompressionETC2)
		enabledFeatures.textureCompressionETC2 = VK_TRUE;

	if (physicalDeviceFeatures.vertexPipelineStoresAndAtomics)
		enabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support vertex pipeline stores and atomics!\n");

	if (physicalDeviceFeatures.fragmentStoresAndAtomics)
		enabledFeatures.fragmentStoresAndAtomics = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support fragment stores and atomics!\n");

	if (physicalDeviceFeatures.shaderStorageImageExtendedFormats)
		enabledFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support shader storage extended formats!\n");

	if (physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat)
		enabledFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support shader storage write without format!\n");

	//enabledFeatures.shaderClipDistance = VK_TRUE;
	//enabledFeatures.shaderCullDistance = VK_TRUE;

	if (physicalDeviceFeatures.geometryShader)
		enabledFeatures.geometryShader = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support geometry shaders!\n");

	if (physicalDeviceFeatures.tessellationShader)
		enabledFeatures.tessellationShader = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support tessellation shaders!\n");

	if (physicalDeviceFeatures.multiViewport)
		enabledFeatures.multiViewport = VK_TRUE;
	else
		Log::Warning("Selected GPU does not support multi viewports!\n");

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	if (instance.GetEnableValidationLayers()) {
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(Instance::ValidationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = Instance::ValidationLayers.data();
	}
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
	Graphics::CheckVk(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice));

	volkLoadDevice(logicalDevice);

	vkGetDeviceQueue(logicalDevice, graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, presentFamily, 0, &presentQueue);
	vkGetDeviceQueue(logicalDevice, computeFamily, 0, &computeQueue);
	vkGetDeviceQueue(logicalDevice, transferFamily, 0, &transferQueue);
}
}
