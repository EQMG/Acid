#pragma once

#ifdef FLOUNDER_PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include <vulkan/vulkan.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb/stb_image.h"

namespace Flounder
{
	class GlfwVulkan
	{
	public:
		static void GlfwVulkan::ErrorCheck(VkResult result)
		{
			if (result < 0)
			{
				switch (result)
				{
				case VK_ERROR_OUT_OF_HOST_MEMORY:
					printf("VK_ERROR_OUT_OF_HOST_MEMORY\n");
					break;
				case VK_ERROR_OUT_OF_DEVICE_MEMORY:
					printf("VK_ERROR_OUT_OF_DEVICE_MEMORY\n");
					break;
				case VK_ERROR_INITIALIZATION_FAILED:
					printf("VK_ERROR_INITIALIZATION_FAILED\n");
					break;
				case VK_ERROR_DEVICE_LOST:
					printf("VK_ERROR_DEVICE_LOST\n");
					break;
				case VK_ERROR_MEMORY_MAP_FAILED:
					printf("VK_ERROR_MEMORY_MAP_FAILED\n");
					break;
				case VK_ERROR_LAYER_NOT_PRESENT:
					printf("VK_ERROR_LAYER_NOT_PRESENT\n");
					break;
				case VK_ERROR_EXTENSION_NOT_PRESENT:
					printf("VK_ERROR_EXTENSION_NOT_PRESENT\n");
					break;
				case VK_ERROR_FEATURE_NOT_PRESENT:
					printf("VK_ERROR_FEATURE_NOT_PRESENT\n");
					break;
				case VK_ERROR_INCOMPATIBLE_DRIVER:
					printf("VK_ERROR_INCOMPATIBLE_DRIVER\n");
					break;
				case VK_ERROR_TOO_MANY_OBJECTS:
					printf("VK_ERROR_TOO_MANY_OBJECTS\n");
					break;
				case VK_ERROR_FORMAT_NOT_SUPPORTED:
					printf("VK_ERROR_FORMAT_NOT_SUPPORTED\n");
					break;
				case VK_ERROR_SURFACE_LOST_KHR:
					printf("VK_ERROR_SURFACE_LOST_KHR\n");
					break;
				case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
					printf("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR\n");
					break;
				case VK_SUBOPTIMAL_KHR:
					printf("VK_SUBOPTIMAL_KHR\n");
					break;
				case VK_ERROR_OUT_OF_DATE_KHR:
					printf("VK_ERROR_OUT_OF_DATE_KHR\n");
					break;
				case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
					printf("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR\n");
					break;
				case VK_ERROR_VALIDATION_FAILED_EXT:
					printf("VK_ERROR_VALIDATION_FAILED_EXT\n");
					break;
				default:
					break;
				}

				assert(false && "Vulkan runtime error.");
			}
		}
	};
}
