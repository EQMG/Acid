#pragma once

#include <string>
#include "Display/Display.hpp"

namespace fl
{
	class FL_EXPORT Screenshot
	{
	public:
		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into the screenshots folder a png image.
		/// </summary>
		///	<param name="filename"> The new width in pixels. </param>
		static void Capture(const std::string &filename);

	private:
		static void InsertImageMemoryBarrier(const VkCommandBuffer &cmdbuffer, const VkImage &image, const VkAccessFlags &srcAccessMask,
											 const VkAccessFlags &dstAccessMask, const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout,
											 const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask, const VkImageSubresourceRange &subresourceRange);
	};
}
