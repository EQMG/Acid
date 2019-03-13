#include "LogicalDevice.hpp"

#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Renderer/Renderer.hpp"
#include "Surface.hpp"

namespace acid
{
LogicalDevice::LogicalDevice(const Instance* instance, const PhysicalDevice* physicalDevice, const Surface* surface)
	: m_instance(instance), m_physicalDevice(physicalDevice), m_surface(surface), m_logicalDevice(VK_NULL_HANDLE), m_supportedQueues(0), m_graphicsFamily(0), m_presentFamily(0), m_computeFamily(0), m_transferFamily(0), m_graphicsQueue(VK_NULL_HANDLE),
	  m_presentQueue(VK_NULL_HANDLE), m_computeQueue(VK_NULL_HANDLE), m_transferQueue(VK_NULL_HANDLE)
{
	CreateQueueIndices();
	CreateLogicalDevice();
}

LogicalDevice::~LogicalDevice()
{
	Renderer::CheckVk(vkDeviceWaitIdle(m_logicalDevice));

	vkDestroyDevice(m_logicalDevice, nullptr);
}

void LogicalDevice::CreateQueueIndices()
{
	uint32_t deviceQueueFamilyPropertyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice->GetPhysicalDevice(), &deviceQueueFamilyPropertyCount, nullptr);
	std::vector<VkQueueFamilyProperties> deviceQueueFamilyProperties(deviceQueueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice->GetPhysicalDevice(), &deviceQueueFamilyPropertyCount, deviceQueueFamilyProperties.data());

	int32_t graphicsFamily = -1;
	int32_t presentFamily = -1;
	int32_t computeFamily = -1;
	int32_t transferFamily = -1;

	for(uint32_t i = 0; i < deviceQueueFamilyPropertyCount; i++)
		{
			// Check for graphics support.
			if(deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					graphicsFamily = i;
					m_graphicsFamily = i;
					m_supportedQueues |= VK_QUEUE_GRAPHICS_BIT;
				}

			// Check for presentation support.
			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice->GetPhysicalDevice(), i, m_surface->GetSurface(), &presentSupport);

			if(deviceQueueFamilyProperties[i].queueCount > 0 && presentSupport)
				{
					presentFamily = i;
					m_presentFamily = i;
				}

			// Check for compute support.
			if(deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
				{
					computeFamily = i;
					m_computeFamily = i;
					m_supportedQueues |= VK_QUEUE_COMPUTE_BIT;
				}

			// Check for transfer support.
			if(deviceQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
				{
					transferFamily = i;
					m_transferFamily = i;
					m_supportedQueues |= VK_QUEUE_TRANSFER_BIT;
				}

			if(graphicsFamily != -1 && presentFamily != -1 && computeFamily != -1 && transferFamily != -1)
				{
					break;
				}
		}

	if(graphicsFamily == -1)
		{
			throw std::runtime_error("Failed to find queue family supporting VK_QUEUE_GRAPHICS_BIT");
		}
}

void LogicalDevice::CreateLogicalDevice()
{
	auto physicalDeviceFeatures = m_physicalDevice->GetFeatures();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = {};
	float queuePriorities[] = {0.0f};

	if(m_supportedQueues & VK_QUEUE_GRAPHICS_BIT)
		{
			VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
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

	if(m_supportedQueues & VK_QUEUE_COMPUTE_BIT && m_computeFamily != m_graphicsFamily)
		{
			VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
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

	if(m_supportedQueues & VK_QUEUE_TRANSFER_BIT && m_transferFamily != m_graphicsFamily && m_transferFamily != m_computeFamily)
		{
			VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
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

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.sampleRateShading = VK_TRUE;
	deviceFeatures.fillModeNonSolid = VK_TRUE;
	deviceFeatures.wideLines = VK_TRUE;
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;
	deviceFeatures.fragmentStoresAndAtomics = VK_TRUE;
	deviceFeatures.shaderStorageImageExtendedFormats = VK_TRUE;
	deviceFeatures.shaderStorageImageWriteWithoutFormat = VK_TRUE;
	deviceFeatures.shaderClipDistance = VK_TRUE;
	deviceFeatures.shaderCullDistance = VK_TRUE;

	if(physicalDeviceFeatures.geometryShader)
		{
			deviceFeatures.geometryShader = VK_TRUE;
		}
	else
		{
			Log::Error("Selected GPU does not support geometry shaders!");
		}

	if(physicalDeviceFeatures.tessellationShader)
		{
			deviceFeatures.tessellationShader = VK_TRUE;
		}
	else
		{
			Log::Error("Selected GPU does not support tessellation shaders!");
		}

	if(physicalDeviceFeatures.multiViewport)
		{
			deviceFeatures.multiViewport = VK_TRUE;
		}
	else
		{
			Log::Error("Selected GPU does not support multi viewports!");
		}

	if(physicalDeviceFeatures.textureCompressionBC)
		{
			deviceFeatures.textureCompressionBC = VK_TRUE;
		}
	else if(physicalDeviceFeatures.textureCompressionASTC_LDR)
		{
			deviceFeatures.textureCompressionASTC_LDR = VK_TRUE;
		}
	else if(physicalDeviceFeatures.textureCompressionETC2)
		{
			deviceFeatures.textureCompressionETC2 = VK_TRUE;
		}

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instance->GetInstanceLayers().size());
	deviceCreateInfo.ppEnabledLayerNames = m_instance->GetInstanceLayers().data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instance->GetDeviceExtensions().size());
	deviceCreateInfo.ppEnabledExtensionNames = m_instance->GetDeviceExtensions().data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	Renderer::CheckVk(vkCreateDevice(m_physicalDevice->GetPhysicalDevice(), &deviceCreateInfo, nullptr, &m_logicalDevice));

	vkGetDeviceQueue(m_logicalDevice, m_graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_logicalDevice, m_presentFamily, 0, &m_presentQueue);
	vkGetDeviceQueue(m_logicalDevice, m_computeFamily, 0, &m_computeQueue);
	vkGetDeviceQueue(m_logicalDevice, m_transferFamily, 0, &m_transferQueue);
}
}
