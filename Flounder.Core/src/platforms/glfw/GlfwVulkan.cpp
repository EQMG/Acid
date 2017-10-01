#include "GlfwVulkan.hpp"

#include <string>
#include <cassert>

namespace Flounder
{
	void GlfwVulkan::ErrorVk(const VkResult &result)
	{
		if (result < 0)
		{
			std::string failure;

			switch (result)
			{
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				failure = "VK_ERROR_OUT_OF_HOST_MEMORY\n";
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				failure = "VK_ERROR_OUT_OF_DEVICE_MEMORY\n";
				break;
			case VK_ERROR_INITIALIZATION_FAILED:
				failure = "VK_ERROR_INITIALIZATION_FAILED\n";
				break;
			case VK_ERROR_DEVICE_LOST:
				failure = "VK_ERROR_DEVICE_LOST\n";
				break;
			case VK_ERROR_MEMORY_MAP_FAILED:
				failure = "VK_ERROR_MEMORY_MAP_FAILED\n";
				break;
			case VK_ERROR_LAYER_NOT_PRESENT:
				failure = "VK_ERROR_LAYER_NOT_PRESENT\n";
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT:
				failure = "VK_ERROR_EXTENSION_NOT_PRESENT\n";
				break;
			case VK_ERROR_FEATURE_NOT_PRESENT:
				failure = "VK_ERROR_FEATURE_NOT_PRESENT\n";
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				failure = "VK_ERROR_INCOMPATIBLE_DRIVER\n";
				break;
			case VK_ERROR_TOO_MANY_OBJECTS:
				failure = "VK_ERROR_TOO_MANY_OBJECTS\n";
				break;
			case VK_ERROR_FORMAT_NOT_SUPPORTED:
				failure = "VK_ERROR_FORMAT_NOT_SUPPORTED\n";
				break;
			case VK_ERROR_SURFACE_LOST_KHR:
				failure = "VK_ERROR_SURFACE_LOST_KHR\n";
				break;
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
				failure = "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR\n";
				break;
			case VK_SUBOPTIMAL_KHR:
				failure = "VK_SUBOPTIMAL_KHR\n";
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
				failure = "VK_ERROR_OUT_OF_DATE_KHR\n";
				break;
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
				failure = "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR\n";
				break;
			case VK_ERROR_VALIDATION_FAILED_EXT:
				failure = "VK_ERROR_VALIDATION_FAILED_EXT\n";
				break;
			default:
				failure = "VK_RESULT_MAX_ENUM";
				break;
			}

#ifdef FLOUNDER_PLATFORM_WINDOWS
			MessageBox(nullptr, failure.c_str(), "Vulkan Error", 0);
#endif
			assert(false && "Vulkan runtime error.");
		}
	}

	void GlfwVulkan::ErrorGlfw(const int &result)
	{
		if (result == GLFW_FALSE)
		{
			assert(false && "GLFW runtime error.");
		}
	}

	uint32_t GlfwVulkan::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties)
	{
		for (uint32_t i = 0; i < deviceMemoryProperties->memoryTypeCount; ++i)
		{
			if (memoryRequirements->memoryTypeBits & (1 << i))
			{
				if ((deviceMemoryProperties->memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties)
				{
					return i;
				}
			}
		}

		assert(false && "Vulkan runtime error, couldn't find proper memory type!");
		return UINT32_MAX;
	}
}
