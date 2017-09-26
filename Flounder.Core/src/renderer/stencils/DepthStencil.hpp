#pragma once

#include <vector>

#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	class DepthStencil
	{
	private:
		VkImage m_depthStencilImage;
		VkDeviceMemory m_depthStencilImageMemory;
		VkImageView m_depthStencilImageView;
		VkFormat m_depthStencilFormat;
	public:
		DepthStencil();

		~DepthStencil();

		void Create(const VkDevice &device, const VkPhysicalDevice &physicalDevice, const VkPhysicalDeviceMemoryProperties &physicalDeviceMemoryProperties, const VkExtent3D &extent);

		void Cleanup(const VkDevice &device);

		VkImage GetImage() const { return m_depthStencilImage; }

		VkDeviceMemory GetImageMemory() const { return m_depthStencilImageMemory; }

		VkImageView GetImageView() const { return m_depthStencilImageView; }

		VkFormat GetFormat() const { return m_depthStencilFormat; }
	};
}
