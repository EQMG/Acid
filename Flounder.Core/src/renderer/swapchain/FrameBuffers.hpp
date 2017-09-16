#pragma once

#include <vector>

#include "../../platforms/glfw/GlfwVulkan.hpp"

namespace Flounder
{
	class FrameBuffers
	{
	private:
		// Vector of handles to framebuffers.
		std::vector<VkFramebuffer> m_swapchainFramebuffers;
	public:
		FrameBuffers();

		~FrameBuffers();

		void Create(const VkDevice *logicalDevice, const VkRenderPass &renderPass, const VkExtent2D &swapchainExtent, const std::vector<VkImageView> &swapchainImageViews);

		void Cleanup(const VkDevice *logicalDevice);

		size_t GetFramebufferSize() const { return m_swapchainFramebuffers.size(); }

		VkFramebuffer GetFramebuffer(const uint32_t &index) const { return m_swapchainFramebuffers[index]; }
	};
}
