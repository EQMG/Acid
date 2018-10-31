#pragma once

#include <string>
#include <vector>
#include <optional>
#include <vulkan/vulkan.h>
#include "Engine/Log.hpp"
#include "Maths/Colour.hpp"

namespace acid
{
	enum AttachmentType
	{
		ATTACHMENT_TYPE_IMAGE = 0,
		ATTACHMENT_TYPE_DEPTH = 1,
		ATTACHMENT_TYPE_SWAPCHAIN = 2
	};

	/// <summary>
	/// A object that represents an attachment in a renderpass.
	/// </summary>
	class ACID_EXPORT Attachment
	{
	private:
		uint32_t m_binding;
		std::string m_name;
		AttachmentType m_type;
		VkFormat m_format;
		bool m_multisampled;
		Colour m_clearColour;
	public:
		/// <summary>
		/// Creates a new attachment that represents a object in the render pipeline.
		/// </summary>
		/// <param name="binding"> The index the attachment is bound to in the renderpass. </param>
		/// <param name="name"> The unique name given to the object for all renderpasses. </param>
		/// <param name="type"> The attachment type this represents. </param>
		/// <param name="format"> The format that will be created (only applies to type ATTACHMENT_IMAGE). </param>
		/// <param name="multisampled"> If this attachment is multisampled. </param>
		/// <param name="clearColour"> The colour to clear to before rendering to it. </param>
		Attachment(const uint32_t &binding, const std::string &name, const AttachmentType &type, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const bool &multisampled = false, const Colour &clearColour = Colour::BLACK) :
			m_binding(binding),
			m_name(name),
			m_type(type),
			m_format(format),
			m_multisampled(multisampled),
			m_clearColour(clearColour)
		{
		}

		uint32_t GetBinding() const { return m_binding; }

		std::string GetName() const { return m_name; }

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
		std::vector<Attachment> m_images;
		std::vector<SubpassType> m_subpasses;
		
		uint32_t m_width;
		uint32_t m_height;
		float m_scale;
	public:
		RenderpassCreate(const std::vector<Attachment> &images = {}, const std::vector<SubpassType> &subpasses = {}, const uint32_t &width = 0, const uint32_t &height = 0, const float &scale = 1.0f) :
			m_images(images),
			m_subpasses(subpasses),
			m_width(width),
			m_height(height),
			m_scale(scale)
		{
		}

		std::vector<Attachment> GetImages() const { return m_images; }

		std::vector<SubpassType> GetSubpasses() const { return m_subpasses; }

		std::optional<uint32_t> GetAttachment(const std::string &name) const
		{
			uint32_t attachment = 0;

			for (auto &image : m_images)
			{
				if (image.GetName() == name)
				{
					return attachment;
				}

				attachment++;
			}

		//	Log::Error("Filed to find a renderpass attachment by name: '%i'\n", name.c_str());
			return {};
		}

		std::optional<uint32_t> GetAttachment(const uint32_t &binding) const
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

		//	Log::Error("Filed to find a renderpass attachment bound to: %i\n", binding);
			return {};
		}
		
		uint32_t GetWidth() const { return m_width; }

		void SetWidth(const uint32_t &width) { m_width = width; }

		uint32_t GetHeight() const { return m_height; }

		void SetHeight(const uint32_t &height) { m_height = height; }

		float GetScale() const { return m_scale; }

		void SetScale(const float &scale) { m_scale = scale; }
	};
}
