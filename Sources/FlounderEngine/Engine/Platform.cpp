#include "Platform.hpp"

#include <string>
#include <stdexcept>

#ifdef FLOUNDER_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include "../Devices/Display.hpp"
#include "../Renderer/Renderer.hpp"

namespace Flounder
{
	void Platform::ErrorVk(const VkResult &result)
	{
		if (result < 0)
		{
			std::string failure;

			switch (result)
			{
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				failure = "VK_ERROR_OUT_OF_HOST_MEMORY";
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				failure = "VK_ERROR_OUT_OF_DEVICE_MEMORY";
				break;
			case VK_ERROR_INITIALIZATION_FAILED:
				failure = "VK_ERROR_INITIALIZATION_FAILED";
				break;
			case VK_ERROR_DEVICE_LOST:
				failure = "VK_ERROR_DEVICE_LOST";
				break;
			case VK_ERROR_MEMORY_MAP_FAILED:
				failure = "VK_ERROR_MEMORY_MAP_FAILED";
				break;
			case VK_ERROR_LAYER_NOT_PRESENT:
				failure = "VK_ERROR_LAYER_NOT_PRESENT";
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT:
				failure = "VK_ERROR_EXTENSION_NOT_PRESENT";
				break;
			case VK_ERROR_FEATURE_NOT_PRESENT:
				failure = "VK_ERROR_FEATURE_NOT_PRESENT";
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				failure = "VK_ERROR_INCOMPATIBLE_DRIVER";
				break;
			case VK_ERROR_TOO_MANY_OBJECTS:
				failure = "VK_ERROR_TOO_MANY_OBJECTS";
				break;
			case VK_ERROR_FORMAT_NOT_SUPPORTED:
				failure = "VK_ERROR_FORMAT_NOT_SUPPORTED";
				break;
			case VK_ERROR_SURFACE_LOST_KHR:
				failure = "VK_ERROR_SURFACE_LOST_KHR";
				break;
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
				failure = "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
				break;
			case VK_SUBOPTIMAL_KHR:
				failure = "VK_SUBOPTIMAL_KHR";
				break;
			case VK_ERROR_OUT_OF_DATE_KHR:
				failure = "VK_ERROR_OUT_OF_DATE_KHR";
				break;
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
				failure = "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
				break;
			case VK_ERROR_VALIDATION_FAILED_EXT:
				failure = "VK_ERROR_VALIDATION_FAILED_EXT";
				break;
			default:
				failure = "VK_RESULT_MAX_ENUM";
				break;
			}

			fprintf(stderr, "Vulkan error: %s, %i\n", failure.c_str(), result);
#ifdef FLOUNDER_PLATFORM_WINDOWS
			MessageBox(nullptr, failure.c_str(), "Vulkan Error", 0);
#endif
			throw std::runtime_error("Vulkan runtime error.");
		}
	}

	void Platform::ErrorSpv(const spv_result_t &result)
	{
		if (result < 0)
		{
			std::string failure;

			switch (result)
			{
			case SPV_ERROR_INTERNAL:
				failure = "SPV_ERROR_INTERNAL";
				break;
			case SPV_ERROR_OUT_OF_MEMORY:
				failure = "SPV_ERROR_OUT_OF_MEMORY";
				break;
			case SPV_ERROR_INVALID_POINTER:
				failure = "SPV_ERROR_INVALID_POINTER";
				break;
			case SPV_ERROR_INVALID_BINARY:
				failure = "SPV_ERROR_INVALID_BINARY";
				break;
			case SPV_ERROR_INVALID_TEXT:
				failure = "SPV_ERROR_INVALID_TEXT";
				break;
			case SPV_ERROR_INVALID_TABLE:
				failure = "SPV_ERROR_INVALID_TABLE";
				break;
			case SPV_ERROR_INVALID_VALUE:
				failure = "SPV_ERROR_INVALID_VALUE";
				break;
			case SPV_ERROR_INVALID_DIAGNOSTIC:
				failure = "SPV_ERROR_INVALID_DIAGNOSTIC";
				break;
			case SPV_ERROR_INVALID_LOOKUP:
				failure = "SPV_ERROR_INVALID_LOOKUP";
				break;
			case SPV_ERROR_INVALID_ID:
				failure = "SPV_ERROR_INVALID_ID";
				break;
			case SPV_ERROR_INVALID_CFG:
				failure = "SPV_ERROR_INVALID_CFG";
				break;
			case SPV_ERROR_INVALID_LAYOUT:
				failure = "SPV_ERROR_INVALID_LAYOUT";
				break;
			case SPV_ERROR_INVALID_CAPABILITY:
				failure = "SPV_ERROR_INVALID_CAPABILITY";
				break;
			case SPV_ERROR_INVALID_DATA:
				failure = "SPV_ERROR_INVALID_DATA";
				break;
			case SPV_ERROR_MISSING_EXTENSION:
				failure = "SPV_ERROR_MISSING_EXTENSION";
				break;
			case SPV_ERROR_WRONG_VERSION:
				failure = "SPV_ERROR_WRONG_VERSION";
				break;
			default:
				failure = "SPV_FORCE_32_BIT_ENUM";
				break;
			}

			fprintf(stderr, "SPIRV error: %s, %i\n", failure.c_str(), result);
#ifdef FLOUNDER_PLATFORM_WINDOWS
			MessageBox(nullptr, failure.c_str(), "SPIRV Error", 0);
#endif
			throw std::runtime_error("SPIRV runtime error.");
		}
	}

	void Platform::ErrorGlfw(const int &result)
	{
		if (result == GLFW_FALSE)
		{
			fprintf(stderr, "GLFW error: %i\n", result);
			throw std::runtime_error("GLFW runtime error.");
		}
	}

	void Platform::ErrorAl(const ALenum &result)
	{
		if (result == AL_NO_ERROR)
		{
			return;
		}

		fprintf(stderr, "OpenAL error: %i\n", result);
#ifdef FLOUNDER_PLATFORM_WINDOWS
		MessageBox(nullptr, "Error: " + result, "OpenAL Error", 0);
#endif
		throw std::runtime_error("OpenAL runtime error.");
	}

	VkCommandBuffer Platform::BeginSingleTimeCommands(const VkCommandBufferLevel &level)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto commandPool = Renderer::Get()->GetCommandPool();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = level;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		Platform::ErrorVk(vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer));

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Platform::ErrorVk(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		return commandBuffer;
	}

	void Platform::EndSingleTimeCommands(const VkCommandBuffer &commandBuffer)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();
		const auto commandPool = Renderer::Get()->GetCommandPool();

		Platform::ErrorVk(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		Platform::ErrorVk(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

		Platform::ErrorVk(vkQueueWaitIdle(queue));

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	}

	uint32_t Platform::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties)
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

#ifdef FLOUNDER_PLATFORM_WINDOWS
		MessageBox(nullptr, "Couldn't find proper memory type!", "Vulkan Error", 0);
#endif
		throw std::runtime_error("Vulkan runtime error, couldn't find proper memory type!");
		return UINT32_MAX;
	}
}
