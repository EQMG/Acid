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

	bool GetEnableValidationLayers() const { return m_enableValidationLayers; }
	const VkInstance &GetInstance() const { return m_instance; }

private:
	bool CheckValidationLayerSupport() const;
	std::vector<const char *> GetExtensions() const;
	void CreateInstance();
	void CreateDebugMessenger();

	static void LogVulkanLayers(const std::vector<VkLayerProperties> &layerProperties);

	bool m_enableValidationLayers = false;

	VkInstance m_instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
};
}
