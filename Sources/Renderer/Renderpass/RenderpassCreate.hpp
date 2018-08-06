#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Maths/Colour.hpp"

namespace acid
{
	enum AttachmentType
	{
		ATTACHMENT_IMAGE = 0,
		ATTACHMENT_DEPTH = 1,
		ATTACHMENT_SWAPCHAIN = 2,
	};

	class ACID_EXPORT Attachment
	{
	private:
		uint32_t m_binding;
		AttachmentType m_type;
		VkFormat m_format;
		Colour m_clearColour;
	public:
		Attachment(const uint32_t &binding, const AttachmentType &type, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const Colour &clearColour = Colour::BLACK) :
			m_binding(binding),
			m_type(type),
			m_format(format),
			m_clearColour(clearColour)
		{
		}

		uint32_t GetBinding() const { return m_binding; }

		AttachmentType GetType() const { return m_type; }

		VkFormat GetFormat() const { return m_format; }

		Colour GetClearColour() const { return m_clearColour; }
	};

	class ACID_EXPORT SubpassType
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

	class ACID_EXPORT RenderpassCreate
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
