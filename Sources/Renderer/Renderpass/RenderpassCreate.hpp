#pragma once

#include <string>
#include <vector>
#include "Display/Display.hpp"
#include "Maths/Colour.hpp"

namespace fl
{
	enum AttachmentType
	{
		TypeImage = 0,
		TypeDepth = 1,
		TypeSwapchain = 2,
	};

	class FL_EXPORT Attachment
	{
	public:
		unsigned int m_binding;
		AttachmentType m_type;
		VkFormat m_format;
		Colour m_clearColour;
		VkImageLayout m_layout;
		VkImageUsageFlags m_usage;

		Attachment(const unsigned int &binding, const AttachmentType &type, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const Colour &clearColour = Colour::BLACK,
				   const VkImageLayout &layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) :
			m_binding(binding),
			m_type(type),
			m_format(format),
			m_clearColour(clearColour),
			m_layout(layout),
			m_usage(usage)
		{
		}
	};

	class FL_EXPORT SubpassType
	{
	public:
		uint32_t m_binding;
		std::vector<uint32_t> m_attachments;

		SubpassType(const uint32_t &binding, const std::vector<uint32_t> &attachments) :
			m_binding(binding),
			m_attachments(std::vector<uint32_t>(attachments))
		{
		}
	};

	struct RenderpassCreate
	{
		uint32_t m_width;
		uint32_t m_height;

		std::vector<Attachment> images = std::vector<Attachment>();
		std::vector<SubpassType> subpasses = std::vector<SubpassType>();
	};
}
