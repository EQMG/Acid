#pragma once

#include "../../platforms/Platform.hpp"

namespace Flounder
{
	class DepthStencil
	{
	private:
		VkImage m_image;
		VkDeviceMemory m_imageMemory;
		VkImageView m_imageView;
		VkFormat m_format;
	public:
		DepthStencil(const VkExtent3D &extent);

		~DepthStencil();

		VkImage GetImage() const { return m_image; }

		VkDeviceMemory GetImageMemory() const { return m_imageMemory; }

		VkImageView GetImageView() const { return m_imageView; }

		VkFormat GetFormat() const { return m_format; }
	};
}
