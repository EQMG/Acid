#pragma once

#include <vulkan/vulkan.h>
#include "StdAfx.hpp"

namespace acid {
// Debug messenger is a newer Vulkan feature, but extremely useful if available.
#define USE_DEBUG_MESSENGER VK_HEADER_VERSION >= 121

class ACID_EXPORT Instance {
	friend class Graphics;
public:
#if USE_DEBUG_MESSENGER
	friend VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
#else
	friend VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode,
		const char *pLayerPrefix, const char *pMessage, void *pUserData);
#endif

	static const std::vector<const char *> ValidationLayers;

	Instance();

	~Instance();

#if USE_DEBUG_MESSENGER
	static VkResult FvkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator,
		VkDebugUtilsMessengerEXT *pDebugMessenger);
	static void FvkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *pAllocator);
#else
	static VkResult FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator,
		VkDebugReportCallbackEXT *pCallback);
	static void FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator);
#endif

	static void FvkCmdPushDescriptorSetKHR(VkDevice device, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
		uint32_t descriptorWriteCount, const VkWriteDescriptorSet *pDescriptorWrites);

	static uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements,
		VkMemoryPropertyFlags requiredProperties);

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
#if USE_DEBUG_MESSENGER
	VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
#else
	VkDebugReportCallbackEXT m_debugReportCallback = VK_NULL_HANDLE;
#endif
};
}
