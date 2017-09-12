#pragma once

#include <vector>

#include "../../platforms/glfw/GlfwVulkan.h"

namespace Flounder
{
	/// <summary>
	/// Used to store data for the swap chain.
	/// </summary>
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class Swapchain
	{
	private:
		const VkDevice* m_logicalDevice;

		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_swapChainImages;
		std::vector<VkImageView> m_swapChainImageViews;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;

		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
	public:
		Swapchain();

		~Swapchain();

		void Create(const VkDevice *logicalDevice, VkPhysicalDevice *physicalDevice, const VkSurfaceKHR *surface, GLFWwindow *window);

		void CreateFramebuffers(const VkDevice* logicalDevice, const VkRenderPass* renderPass);

		void Cleanup();

		void CleanupFrameBuffers();

		VkSwapchainKHR GetSwapchain() const { return m_swapChain; }

		size_t GetFramebufferSize() const { return m_swapChainFramebuffers.size(); }

		VkFramebuffer GetFramebuffer(const uint32_t &index) const { return m_swapChainFramebuffers[index]; }

		VkFormat GetImageFormat() const { return m_swapChainImageFormat; }

		VkExtent2D GetExtent() const { return m_swapChainExtent; }
	private:
		void CreateImageViews(const VkDevice* logicalDevice);

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const;

		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const;

		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) const;
	};
}
