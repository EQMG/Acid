#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <AL/al.h>
#include <AL/alc.h>

namespace Flounder
{
	class Platform
	{
	public:
		static void ErrorVk(const VkResult &result);

		static void ErrorGlfw(const int &result);

		static void ErrorAl(const ALenum &result);

		static uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties);
	};
}
