#pragma once

#include <string>
#include <vector>
#include <optional>
#include <algorithm>
#include <vulkan/vulkan.h>
#include "Engine/Log.hpp"
#include "Maths/Colour.hpp"
#include "Maths/Vector2.hpp"

namespace acid
{
	/// <summary>
	/// A object that represents an attachment in a renderpass.
	/// </summary>
	class ACID_EXPORT Attachment
	{
	public:
		enum class Type
		{
			Image,
			Depth,
			Swapchain
		};

		/// <summary>
		/// Creates a new attachment that represents a object in the render pipeline.
		/// </summary>
		/// <param name="binding"> The index the attachment is bound to in the renderpass. </param>
		/// <param name="name"> The unique name given to the object for all renderpasses. </param>
		/// <param name="multisampled"> If this attachment is multisampled. </param>
		/// <param name="type"> The attachment type this represents. </param>
		/// <param name="format"> The format that will be created (only applies to type ATTACHMENT_IMAGE). </param>
		/// <param name="clearColour"> The colour to clear to before rendering to it. </param>
		Attachment(const uint32_t &binding, const std::string &name, const Type &type, const bool &multisampled = false, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const Colour &clearColour = Colour::Black) :
			m_binding(binding),
			m_name(name),
			m_type(type),
			m_multisampled(multisampled),
			m_format(format),
			m_clearColour(clearColour)
		{
		}

		const uint32_t &GetBinding() const { return m_binding; }

		const std::string &GetName() const { return m_name; }

		const Type &GetType() const { return m_type; }

		const bool &IsMultisampled() const { return m_multisampled; }

		const VkFormat &GetFormat() const { return m_format; }

		const Colour &GetClearColour() const { return m_clearColour; }
	private:
		uint32_t m_binding;
		std::string m_name;
		Type m_type;
		bool m_multisampled;
		VkFormat m_format;
		Colour m_clearColour;
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

		const uint32_t &GetBinding() const { return m_binding; }

		const std::vector<uint32_t> &GetAttachmentBindings() const { return m_attachmentBindings; }
	};

	class ACID_EXPORT RenderpassCreate
	{
	private:
		std::vector<Attachment> m_images;
		std::vector<SubpassType> m_subpasses;

		std::optional<uint32_t> m_width;
		std::optional<uint32_t> m_height;
		Vector2 m_scale;
		Vector2 m_offset;
	public:
		RenderpassCreate(const std::vector<Attachment> &images = {}, const std::vector<SubpassType> &subpasses = {}, const std::optional<uint32_t> &width = {}, const std::optional<uint32_t> &height = {}) :
			m_images(images),
			m_subpasses(subpasses),
			m_width(width),
			m_height(height),
			m_scale(Vector2::One),
			m_offset(Vector2::Zero)
		{
		}

		std::optional<Attachment> GetAttachment(const std::string &name) const
		{
			auto it = std::find_if(m_images.begin(), m_images.end(), [name](const Attachment &a)
			{
				return a.GetName() == name;
			});

			if (it == m_images.end())
			{
				return {};
			}

			return *it;
		}

		std::optional<Attachment> GetAttachment(const uint32_t &binding) const
		{
			auto it = std::find_if(m_images.begin(), m_images.end(), [binding](const Attachment &a)
			{
				return a.GetBinding() == binding;
			});

			if (it == m_images.end())
			{
				return {};
			}

			return *it;
		}

		const std::vector<Attachment> &GetImages() const { return m_images; }

		const std::vector<SubpassType> &GetSubpasses() const { return m_subpasses; }

		const std::optional<uint32_t> &GetWidth() const { return m_width; }

		void SetWidth(const std::optional<uint32_t> &width) { m_width = width; }

		const std::optional<uint32_t> &GetHeight() const { return m_height; }

		void SetHeight(const std::optional<uint32_t> &height) { m_height = height; }

		const Vector2 &GetScale() const { return m_scale; }

		void SetScale(const Vector2 &scale) { m_scale = scale; }

		const Vector2 &GetOffset() const { return m_offset; }

		void SetOffset(const Vector2 &offset) { m_offset = offset; }
	};
}
