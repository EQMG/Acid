#include "Screenshot.hpp"

#include <algorithm>
#include "Helpers/FileSystem.hpp"
#include "Renderer/Renderer.hpp"

namespace acid
{
	void Screenshot::Capture(const std::string &filename)
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto surfaceFormat = Display::Get()->GetVkSurfaceFormat();
		auto width = Display::Get()->GetWidth();
		auto height = Display::Get()->GetHeight();

		fprintf(stdout, "Saving screenshot to: '%s'\n", filename.c_str());

		VkImage srcImage = Renderer::Get()->GetSwapchain()->GetVkImages().at(Renderer::Get()->GetVkActiveSwapchainImage());
		VkImage dstImage;
		VkDeviceMemory dstImageMemory;
		bool supportsBlit = Texture::CopyImage(srcImage, dstImage, dstImageMemory, width, height);

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

		// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'll have to manually swizzle color components
		bool colourSwizzle = false;

		// Check if source is BGR.
		if (!supportsBlit)
		{
			std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
			colourSwizzle = std::find(formatsBGR.begin(), formatsBGR.end(), surfaceFormat.format) != formatsBGR.end();
		}

		// TODO: If colourSwizzle then swap B and R values.

		// Writes the image.
		Texture::WritePixels(filename, data, width, height, 4);

		// Clean up resources.
		vkUnmapMemory(logicalDevice, dstImageMemory);
		vkFreeMemory(logicalDevice, dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice, dstImage, nullptr);

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Screenshot '%s' saved in %fms\n", filename.c_str(), debugEnd - debugStart);
#endif
	}
}
