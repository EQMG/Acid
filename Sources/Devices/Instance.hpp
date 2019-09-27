#pragma once

#include <vulkan/vulkan.h>
#include "StdAfx.hpp"

namespace acid {
class ACID_EXPORT Instance {
	friend class Graphics;
public:
	friend VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

	static const std::vector<const char *> ValidationLayers;
	static const std::vector<const char *> InstanceExtensions;
	static const std::vector<const char *> DeviceExtensions;

	Instance();

	~Instance();

	static VkResult FvkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, \
		VkDebugUtilsMessengerEXT *pDebugMessenger);
	static void FvkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator);

	static void FvkCmdPushDescriptorSetKHR(VkDevice device, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
		uint32_t descriptorWriteCount, const VkWriteDescriptorSet *pDescriptorWrites);

	static uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements,
		const VkMemoryPropertyFlags &requiredProperties);

	operator const VkInstance &() const { return m_instance; }

	const std::vector<const char *> &GetInstanceLayers() const { return m_instanceLayers; }
	const std::vector<const char *> &GetInstanceExtensions() const { return m_instanceExtensions; }
	const std::vector<const char *> &GetDeviceExtensions() const { return m_deviceExtensions; }
	const VkInstance &GetInstance() const { return m_instance; }

private:
	void SetupLayers();
	void SetupExtensions();
	void CreateInstance();
	void CreateDebugCallback();

	static void LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties);

	std::vector<const char *> m_instanceLayers;
	std::vector<const char *> m_instanceExtensions;
	std::vector<const char *> m_deviceExtensions;

	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
	VkInstance m_instance = VK_NULL_HANDLE;
};
}
