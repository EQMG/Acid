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
	private:
		unsigned int m_binding;
		AttachmentType m_type;
		VkFormat m_format;
		Colour m_clearColour;
		VkImageLayout m_layout;
		VkImageUsageFlags m_usage;
	public:
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

		unsigned int GetBinding() const { return m_binding; }

		AttachmentType GetType() const { return m_type; }

		VkFormat GetFormat() const { return m_format; }

		Colour GetClearColour() const { return m_clearColour; }

		VkImageLayout GetLayout() const { return m_layout; }

		VkImageUsageFlags GetUsage() const { return m_usage; }
	};

	class FL_EXPORT SubpassType
	{
	private:
		uint32_t m_binding;
		std::vector<uint32_t> m_attachments;
	public:
		SubpassType(const uint32_t &binding, const std::vector<uint32_t> &attachments) :
			m_binding(binding),
			m_attachments(std::vector<uint32_t>(attachments))
		{
		}

		uint32_t GetBinding() const { return m_binding; }

		std::vector<uint32_t> GetAttachments() const { return m_attachments; }
	};

	class FL_EXPORT RenderpassCreate
	{
	private:
		uint32_t m_width;
		uint32_t m_height;

		std::vector<Attachment> m_images;
		std::vector<SubpassType> m_subpasses;
	public:
		RenderpassCreate(const uint32_t &width, const uint32_t &height, const std::vector<Attachment> &images = std::vector<Attachment>(), const std::vector<SubpassType> &subpasses = std::vector<SubpassType>()) :
			m_width(width),
			m_height(height),
			m_images(images),
			m_subpasses(subpasses)
		{
		}

		uint32_t GetWidth() const { return m_width; }

		void SetWidth(const uint32_t &width) { m_width = width; }

		uint32_t GetHeight() const { return m_height; }

		void SetHeight(const uint32_t &height) { m_height = height; }

		std::vector<Attachment> GetImages() const { return m_images; }

		std::vector<SubpassType> GetSubpasses() const { return m_subpasses; }
	};
}
