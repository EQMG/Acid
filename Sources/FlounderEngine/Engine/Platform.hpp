#pragma once

#include <STB/stb_image.h>
#include <STB/stb_image_write.h>
#include <STB/stb_vorbis.h>

#ifdef FLOUNDER_PLATFORM_MACOS
#include <MoltenVK/vulkan/vulkan.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <vulkan/vulkan.h>
#include <AL/al.h>
#include <AL/alc.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../Prerequisites.hpp"

namespace Flounder
{
	class F_EXPORT Platform
	{
	public:
		static void ErrorVk(const VkResult &result);

		static void ErrorGlfw(const int &result);

		static void ErrorAl(const ALenum &result);

		static VkCommandBuffer BeginSingleTimeCommands(const VkCommandBufferLevel &level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		static void EndSingleTimeCommands(const VkCommandBuffer &commandBuffer);

		static uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties);
	};
}
