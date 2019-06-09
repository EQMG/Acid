#include "LogicalDevice.hpp"

#include "Graphics/Graphics.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

namespace acid
{
LogicalDevice::LogicalDevice(const Instance *instance, const PhysicalDevice *physicalDevice, const Surface *surface) :
	m_instance{instance},
	m_physicalDevice{physicalDevice},
	m_surface{surface}
{
	CreateQueueIndices();
	CreateLogicalDevice();
}

LogicalDevice::~LogicalDevice()
{
	Graphics::CheckVk(vkDeviceWaitIdle(m_logicalDevice));

	vkDestroyDevice(m_logicalDevice, nullptr);
}

void LogicalDevice::CreateQueueIndices()
{
	uint32_t deviceQueueFamilyPropertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(*m_physicalDevice, &deviceQueueFamilyPropertyCount, nullptr);
	std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties{deviceQueueFamilyPropertyCount};
	vkGetPhysicalDeviceQueueFamilyProperties(*m_physicalDevice, &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

	std::optional<uint32_t> graphicsFamily, presentFamily, computeFamily, transferFamily;

	for (uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++)
	{
		// Check for graphics support.
		if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			graphicsFamily = i;
			m_graphicsFamily = i;
			m_supportedQueues |= VK_QUEUE_GRAPHICS_BIT;
		}

		// Check for presentation support.
		VkBool32 presentSupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR(*m_physicalDevice, i, *m_surface, &presentSupport);

		if (deviceQueueFamilyProperties[i].queueCount > 0 && presentSupport)
		{
			presentFamily = i;
			m_presentFamily = i;
		}

		// Check for compute support.
		if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
		{
			computeFamily = i;
			m_computeFamily = i;
			m_supportedQueues |= VK_QUEUE_COMPUTE_BIT;
		}

		// Check for transfer support.
		if (deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
		{
			transferFamily = i;
			m_transferFamily = i;
			m_supportedQueues |= VK_QUEUE_TRANSFER_BIT;
		}

		if (graphicsFamily && presentFamily && computeFamily && transferFamily)
		{
			break;
		}
	}

	if (!graphicsFamily)
	{
		throw std::runtime_error("Failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT");
	}
}

void LogicalDevice::CreateLogicalDevice()
{
	auto physicalDeviceFeatures{m_physicalDevice->GetFeatures()};

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriorities[]{ 0.0f };

	if (m_supportedQueues & VK_QUEUE_GRAPHICS_BIT)
	{
		VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
		graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueCreateInfo.queueFamilyIndex = m_graphicsFamily;
		graphicsQueueCreateInfo.queueCount = 1;
		graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.emplace_back(graphicsQueueCreateInfo);
	}
	else
	{
		m_graphicsFamily = VK_NULL_HANDLE;
	}

	if (m_supportedQueues & VK_QUEUE_COMPUTE_BIT && m_computeFamily != m_graphicsFamily)
	{
		VkDeviceQueueCreateInfo computeQueueCreateInfo{};
		computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		computeQueueCreateInfo.queueFamilyIndex = m_computeFamily;
		computeQueueCreateInfo.queueCount = 1;
		computeQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.emplace_back(computeQueueCreateInfo);
	}
	else
	{
		m_computeFamily = m_graphicsFamily;
	}

	if (m_supportedQueues & VK_QUEUE_TRANSFER_BIT && m_transferFamily != m_graphicsFamily && m_transferFamily != m_computeFamily)
	{
		VkDeviceQueueCreateInfo transferQueueCreateInfo{};
		transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transferQueueCreateInfo.queueFamilyIndex = m_transferFamily;
		transferQueueCreateInfo.queueCount = 1;
		transferQueueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfos.emplace_back(transferQueueCreateInfo);
	}
	else
	{
		m_transferFamily = m_graphicsFamily;
	}

	VkPhysicalDeviceFeatures enabledFeatures{};

	// Enable sample rate shading filtering if supported.
	if (physicalDeviceFeatures.sampleRateShading)
	{
		enabledFeatures.sampleRateShading = VK_TRUE;
	}

	// Fill mode non solid is required for wireframe display.
	if (physicalDeviceFeatures.fillModeNonSolid)
	{
		enabledFeatures.fillModeNonSolid = VK_TRUE;

		// Wide lines must be present for line width > 1.0f.
		if (physicalDeviceFeatures.wideLines)
		{
			enabledFeatures.wideLines = VK_TRUE;
		}
	}
	else
	{
		Log::Warning("Selected GPU does not support wireframe pipelines!\n");
	}

	if (physicalDeviceFeatures.samplerAnisotropy)
	{
		enabledFeatures.samplerAnisotropy = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support sampler anisotropy!\n");
	}

	if (physicalDeviceFeatures.textureCompressionBC)
	{
		enabledFeatures.textureCompressionBC = VK_TRUE;
	}
	else if (physicalDeviceFeatures.textureCompressionASTC_LDR)
	{
		enabledFeatures.textureCompressionASTC_LDR = VK_TRUE;
	}
	else if (physicalDeviceFeatures.textureCompressionETC2)
	{
		enabledFeatures.textureCompressionETC2 = VK_TRUE;
	}

	if (physicalDeviceFeatures.vertexPipelineStoresAndAtomics)
	{
		enabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support vertex pipeline stores and atomics!\n");
	}

	if (physicalDeviceFeatures.fragmentStoresAndAtomics)
	{
		enabledFeatures.fragmentStoresAndAtomics = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support fragment stores and atomics!\n");
	}

	if (physicalDeviceFeatures.shaderStorageImageExtendedFormats)
	{
		enabledFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support shader storage extended formats!\n");
	}

	if (physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat)
	{
		enabledFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support shader storage write without format!\n");
	}

	//enabledFeatures.shaderClipDistance = VK_TRUE;
	//enabledFeatures.shaderCullDistance = VK_TRUE;

	if (physicalDeviceFeatures.geometryShader)
	{
		enabledFeatures.geometryShader = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support geometry shaders!\n");
	}

	if (physicalDeviceFeatures.tessellationShader)
	{
		enabledFeatures.tessellationShader = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support tessellation shaders!\n");
	}

	if (physicalDeviceFeatures.multiViewport)
	{
		enabledFeatures.multiViewport = VK_TRUE;
	}
	else
	{
		Log::Warning("Selected GPU does not support multi viewports!\n");
	}

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instance->GetInstanceLayers().size());
	deviceCreateInfo.ppEnabledLayerNames = m_instance->GetInstanceLayers().data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instance->GetDeviceExtensions().size());
	deviceCreateInfo.ppEnabledExtensionNames = m_instance->GetDeviceExtensions().data();
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
	Graphics::CheckVk(vkCreateDevice(*m_physicalDevice, &deviceCreateInfo, nullptr, &m_logicalDevice));

	vkGetDeviceQueue(m_logicalDevice, m_graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicalDevice, m_presentFamily, 0, &m_presentQueue);
	vkGetDeviceQueue(m_logicalDevice, m_computeFamily, 0, &m_computeQueue);
	vkGetDeviceQueue(m_logicalDevice, m_transferFamily, 0, &m_transferQueue);
}
}
