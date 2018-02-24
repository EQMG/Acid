#pragma once

#include <string>
#include <vector>
#include "../../Engine/Platform.hpp"

namespace Flounder
{
	class F_EXPORT ImageAttachment
	{
	public:
		int m_binding;
		VkFormat m_format;
		VkImageLayout m_layout;
		VkImageUsageFlags m_usage;
		VkClearColorValue m_clearColour;
		uint32_t m_width;
		uint32_t m_height;

		ImageAttachment(const int &binding, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
						const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, const VkClearColorValue &clearColour = {{0.0f, 0.0f, 0.0f, 0.0f}},
						const uint32_t &width = 0, const uint32_t &height = 0) :
			m_binding(binding),
			m_format(format),
			m_layout(layout),
			m_usage(usage),
			m_clearColour(clearColour),
			m_width(width),
			m_height(height)
		{
		}
	};

	class F_EXPORT SubpassType
	{
	public:
		uint32_t m_binding;
		bool m_useDepth;
		std::vector<uint32_t> m_attachments;

		SubpassType(const uint32_t &binding, const bool &useDepth, const std::vector<uint32_t> &attachments) :
			m_binding(binding),
			m_useDepth(useDepth),
			m_attachments(std::vector<uint32_t>(attachments))
		{
		}
	};

	struct RenderpassCreate
	{
		std::vector<ImageAttachment> images = std::vector<ImageAttachment>();
		std::vector<SubpassType> subpasses = std::vector<SubpassType>();
	};
}
