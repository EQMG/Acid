#include "Screenshot.hpp"

#include <fstream>
#include "Helpers/FileSystem.hpp"
#include "Renderer/Renderer.hpp"

namespace fl
{
	void Screenshot::Capture(const std::string &filename)
	{
#if FL_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto physicalDevice = Display::Get()->GetPhysicalDevice();
		const auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		const auto physicalDeviceMemoryProperties = Display::Get()->GetPhysicalDeviceMemoryProperties();
		const auto width = Display::Get()->GetWidth();
		const auto height = Display::Get()->GetHeight();
		VkImage srcImage = Renderer::Get()->GetSwapchain()->GetImages().at(Renderer::Get()->GetActiveSwapchainImage());

		printf("Saving screenshot to: '%s'\n", filename.c_str());

		// Checks blit swapchain support.
		bool supportsBlit = true;
		VkFormatProperties formatProperties;

		// Check if the device supports blitting from optimal images (the swapchain images are in optimal format).
		vkGetPhysicalDeviceFormatProperties(physicalDevice, surfaceFormat.format, &formatProperties);

		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT))
		{
			supportsBlit = false;
		}

		// Check if the device supports blitting to linear images.
		vkGetPhysicalDeviceFormatProperties(physicalDevice, VK_FORMAT_R8G8B8A8_UNORM, &formatProperties);

		if (!(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT))
		{
			supportsBlit = false;
		}

		// Create the linear tiled destination image to copy to and to read the memory from.
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		imageCreateInfo.extent.width = static_cast<uint32_t>(width);
		imageCreateInfo.extent.height = static_cast<uint32_t>(height);
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
		imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		// Create the image.
		VkImage dstImage;
		Display::ErrorVk(vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &dstImage));

		// Create memory to back up the image.
		VkMemoryRequirements memoryRequirements;
		VkMemoryAllocateInfo memoryAllocateInfo{};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

		VkDeviceMemory dstImageMemory;
		vkGetImageMemoryRequirements(logicalDevice, dstImage, &memoryRequirements);
		memoryAllocateInfo.allocationSize = memoryRequirements.size;

		// Memory must be host visible to copy from.
		memoryAllocateInfo.memoryTypeIndex = Renderer::FindMemoryTypeIndex(&physicalDeviceMemoryProperties, &memoryRequirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		Display::ErrorVk(vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &dstImageMemory));
		Display::ErrorVk(vkBindImageMemory(logicalDevice, dstImage, dstImageMemory, 0));

		// Do the actual blit from the swapchain image to our host visible destination image.
		VkCommandBuffer copyCmd = Renderer::BeginSingleTimeCommands(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		// Transition destination image to transfer destination layout.
		InsertImageMemoryBarrier(
			copyCmd,
			dstImage,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_GENERAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		// Transition swapchain image from present to transfer source layout
		InsertImageMemoryBarrier(
			copyCmd,
			srcImage,
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		// If source and destination support blit we'll blit as this also does automatic format conversion (e.g. from BGR to RGB).
		if (supportsBlit)
		{
			// Define the region to blit (we will blit the whole swapchain image).
			VkOffset3D blitSize;
			blitSize.x = width;
			blitSize.y = height;
			blitSize.z = 1;
			VkImageBlit imageBlitRegion{};
			imageBlitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.srcSubresource.layerCount = 1;
			imageBlitRegion.srcOffsets[1] = blitSize;
			imageBlitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlitRegion.dstSubresource.layerCount = 1;
			imageBlitRegion.dstOffsets[1] = blitSize;

			// Issue the blit command
			vkCmdBlitImage(copyCmd, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlitRegion, VK_FILTER_NEAREST);
		}
		else
		{
			// Otherwise use image copy (requires us to manually flip components).
			VkImageCopy imageCopyRegion{};
			imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.srcSubresource.layerCount = 1;
			imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.dstSubresource.layerCount = 1;
			imageCopyRegion.extent.width = static_cast<uint32_t>(width);
			imageCopyRegion.extent.height = static_cast<uint32_t>(height);
			imageCopyRegion.extent.depth = 1;

			// Issue the copy command.
			vkCmdCopyImage(copyCmd, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageCopyRegion);
		}

		// Transition destination image to general layout, which is the required layout for mapping the image memory later on
		InsertImageMemoryBarrier(
			copyCmd,
			dstImage,
			VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_GENERAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		// Transition back the swap chain image after the blit is done
		InsertImageMemoryBarrier(
			copyCmd,
			srcImage,
			VK_ACCESS_TRANSFER_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VkImageSubresourceRange{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1});

		Renderer::EndSingleTimeCommands(copyCmd);

		// Get layout of the image (including row pitch).
		VkImageSubresource subResource{};
		subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		VkSubresourceLayout subResourceLayout;
		vkGetImageSubresourceLayout(logicalDevice, dstImage, &subResource, &subResourceLayout);

		// Creates the screenshot image file.
		FileSystem::CreateFile(filename);

		// Map image memory so we can start copying from it.
		char *data;
		vkMapMemory(logicalDevice, dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void **) &data);
		data += subResourceLayout.offset;

		stbi_write_png(filename.c_str(), width, height, 4, data, width * 4);

		// Clean up resources.
		vkUnmapMemory(logicalDevice, dstImageMemory);
		vkFreeMemory(logicalDevice, dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice, dstImage, nullptr);
		delete[] data;

#if FL_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Screenshot '%s' saved in %fms\n", filename.c_str(), debugEnd - debugStart);
#endif
	}

	void Screenshot::InsertImageMemoryBarrier(const VkCommandBuffer &cmdbuffer, const VkImage &image, const VkAccessFlags &srcAccessMask, const VkAccessFlags &dstAccessMask, const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout, const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask, const VkImageSubresourceRange &subresourceRange)
	{
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);
	}
}
