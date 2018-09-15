#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Engine/Log.hpp"
#include "Maths/Colour.hpp"

namespace acid
{
	enum AttachmentType
	{
		ATTACHMENT_IMAGE = 0,
		ATTACHMENT_DEPTH = 1,
		ATTACHMENT_SWAPCHAIN = 2
	};

	class ACID_EXPORT Attachment
	{
	private:
		uint32_t m_binding;
		AttachmentType m_type;
		VkFormat m_format;
		bool m_multisampled;
		Colour m_clearColour;
	public:
		Attachment(const uint32_t &binding, const AttachmentType &type, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const bool &multisampled = false, const Colour &clearColour = Colour::BLACK) :
			m_binding(binding),
			m_type(type),
			m_format(format),
			m_multisampled(multisampled),
			m_clearColour(clearColour)
		{
		}

		uint32_t GetBinding() const { return m_binding; }

		AttachmentType GetType() const { return m_type; }

		VkFormat GetFormat() const { return m_format; }

		bool IsMultisampled() const { return m_multisampled; }

		Colour GetClearColour() const { return m_clearColour; }
	};

	class ACID_EXPORT SubpassType
	{
	private:
		uint32_t m_binding;
		std::vector<uint32_t> m_attachmentBindings;
	public:
		SubpassType(const uint32_t &binding, const std::vector<uint32_t> &attachmentBindings) :
			m_binding(binding),
			m_attachmentBindings(std::vector<uint32_t>(attachmentBindings))
		{
		}

		uint32_t GetBinding() const { return m_binding; }

		std::vector<uint32_t> GetAttachmentBindings() const { return m_attachmentBindings; }
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

		uint32_t GetAttachment(const uint32_t &binding) const
		{
			uint32_t attachment = 0;

			for (auto &image : m_images)
			{
				if (image.GetBinding() == binding)
				{
					return attachment;
				}

				attachment++;
			}

			Log::Error("Filed to find a renderpass attachment bound to: %i\n", binding);
			return 0;
		}
	};
}
