#pragma once

#include <string>
#include <vector>
#include "Display/Display.hpp"
#include "Maths/Colour.hpp"

namespace fl
{
	enum AttachmentType
	{
		ATTACHMENT_IMAGE = 0,
		ATTACHMENT_DEPTH = 1,
		ATTACHMENT_SWAPCHAIN = 2,
	};

	enum AttachmentFormat
	{
		FORMAT_R8_UNORM = 9,
		FORMAT_R8_SNORM = 10,
		FORMAT_R8_UINT = 13,
		FORMAT_R8_SINT = 14,
		FORMAT_R8_SRGB = 15,
		FORMAT_R8G8_UNORM = 16,
		FORMAT_R8G8_SNORM = 17,
		FORMAT_R8G8_UINT = 20,
		FORMAT_R8G8_SINT = 21,
		FORMAT_R8G8_SRGB = 22,
		FORMAT_R8G8B8_UNORM = 23,
		FORMAT_R8G8B8_SNORM = 24,
		FORMAT_R8G8B8_UINT = 27,
		FORMAT_R8G8B8_SINT = 28,
		FORMAT_R8G8B8_SRGB = 29,
		FORMAT_B8G8R8_UNORM = 30,
		FORMAT_B8G8R8_SNORM = 31,
		FORMAT_B8G8R8_UINT = 34,
		FORMAT_B8G8R8_SINT = 35,
		FORMAT_B8G8R8_SRGB = 36,
		FORMAT_R8G8B8A8_UNORM = 37,
		FORMAT_R8G8B8A8_SNORM = 38,
		FORMAT_R8G8B8A8_UINT = 41,
		FORMAT_R8G8B8A8_SINT = 42,
		FORMAT_R8G8B8A8_SRGB = 43,
		FORMAT_B8G8R8A8_UNORM = 44,
		FORMAT_B8G8R8A8_SNORM = 45,
		FORMAT_B8G8R8A8_UINT = 48,
		FORMAT_B8G8R8A8_SINT = 49,
		FORMAT_B8G8R8A8_SRGB = 50,
		FORMAT_R16_UNORM = 70,
		FORMAT_R16_SNORM = 71,
		FORMAT_R16_UINT = 74,
		FORMAT_R16_SINT = 75,
		FORMAT_R16_SFLOAT = 76,
		FORMAT_R16G16_UNORM = 77,
		FORMAT_R16G16_SNORM = 78,
		FORMAT_R16G16_UINT = 81,
		FORMAT_R16G16_SINT = 82,
		FORMAT_R16G16_SFLOAT = 83,
		FORMAT_R16G16B16_UNORM = 84,
		FORMAT_R16G16B16_SNORM = 85,
		FORMAT_R16G16B16_UINT = 88,
		FORMAT_R16G16B16_SINT = 89,
		FORMAT_R16G16B16_SFLOAT = 90,
		FORMAT_R16G16B16A16_UNORM = 91,
		FORMAT_R16G16B16A16_SNORM = 92,
		FORMAT_R16G16B16A16_UINT = 95,
		FORMAT_R16G16B16A16_SINT = 96,
		FORMAT_R16G16B16A16_SFLOAT = 97,
		FORMAT_R32_UINT = 98,
		FORMAT_R32_SINT = 99,
		FORMAT_R32_SFLOAT = 100,
		FORMAT_R32G32_UINT = 101,
		FORMAT_R32G32_SINT = 102,
		FORMAT_R32G32_SFLOAT = 103,
		FORMAT_R32G32B32_UINT = 104,
		FORMAT_R32G32B32_SINT = 105,
		FORMAT_R32G32B32_SFLOAT = 106,
		FORMAT_R32G32B32A32_UINT = 107,
		FORMAT_R32G32B32A32_SINT = 108,
		FORMAT_R32G32B32A32_SFLOAT = 109,
		FORMAT_D16_UNORM = 124,
		FORMAT_D32_SFLOAT = 126,
		FORMAT_S8_UINT = 127
	};

	class FL_EXPORT Attachment
	{
	private:
		unsigned int m_binding;
		AttachmentType m_type;
		AttachmentFormat m_format;
		Colour m_clearColour;
	public:
		Attachment(const unsigned int &binding, const AttachmentType &type, const AttachmentFormat &format = FORMAT_R8G8B8A8_UNORM, const Colour &clearColour = Colour::BLACK) :
			m_binding(binding),
			m_type(type),
			m_format(format),
			m_clearColour(clearColour)
		{
		}

		unsigned int GetBinding() const { return m_binding; }

		AttachmentType GetType() const { return m_type; }

		AttachmentFormat GetFormat() const { return m_format; }

		Colour GetClearColour() const { return m_clearColour; }
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
