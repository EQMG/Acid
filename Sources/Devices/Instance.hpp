#pragma once

#include "Engine/Engine.hpp"
#include <vulkan/vulkan.h>

namespace acid
{
class ACID_EXPORT Instance
{
  public:
	friend VKAPI_ATTR VkBool32 VKAPI_CALL CallbackDebug(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);

	static const std::vector<const char*> ValidationLayers;
	static const std::vector<const char*> InstanceExtensions;
	static const std::vector<const char*> DeviceExtensions;

	Instance();

	~Instance();

	static VkResult FvkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);

	static void FvkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);

	static void FvkCmdPushDescriptorSetKHR(VkDevice device, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);

	static uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties* deviceMemoryProperties, const VkMemoryRequirements* memoryRequirements, const VkMemoryPropertyFlags& requiredProperties);

	const std::vector<const char*>& GetInstanceLayers() const
	{
		return m_instanceLayers;
	}

	const std::vector<const char*>& GetInstanceExtensions() const
	{
		return m_instanceExtensions;
	}

	const std::vector<const char*>& GetDeviceExtensions() const
	{
		return m_deviceExtensions;
	}

	const VkInstance& GetInstance() const
	{
		return m_instance;
	}

  private:
	friend class Renderer;

	void SetupLayers();

	void SetupExtensions();

	void CreateInstance();

	void CreateDebugCallback();

	static void LogVulkanLayers(const std::vector<VkLayerProperties>& layerProperties);

	std::vector<const char*> m_instanceLayers;
	std::vector<const char*> m_instanceExtensions;
	std::vector<const char*> m_deviceExtensions;

	VkDebugReportCallbackEXT m_debugReportCallback;
	VkInstance m_instance;
};
}
