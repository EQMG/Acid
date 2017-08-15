#pragma once

#include "../devices/display.h"
#include "../framework/framework.h"
#include "../shaders/shader.h"
#include "../textures/texture.h"

#include "imanagerrender.h"

namespace flounder
{
	class renderer :
		public imodule
	{
	private:
		struct testubo
		{
			colour colour;
		};

		imanagerrender *m_managerRender;

		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_swapChainImages;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		std::vector<VkImageView> m_swapChainImageViews;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;

		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_graphicsPipeline;

		VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;

		VkSemaphore m_imageAvailableSemaphore;
		VkSemaphore m_renderFinishedSemaphore;

		shader *m_shaderTest;

		int lastWidth;
		int lastHeight;
	public:
		/// <summary>
		/// Gets this framework instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static inline renderer *get()
		{
			return static_cast<renderer*>(framework::get()->getInstance("renderer"));
		}

		/// <summary>
		/// Creates a new renderer module.
		/// </summary>
		renderer();

		/// <summary>
		/// Deconstructor for the renderer module.
		/// </summary>
		~renderer();

		void update() override;

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		inline imanagerrender *getManager() { return m_managerRender; }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		inline void setManager(imanagerrender *managerRender) { m_managerRender = managerRender; }

		std::vector<VkCommandBuffer> getVkCommandBuffers() const
		{
			return m_commandBuffers;
		}
	private:
		void createSwapChain();

		void createImageViews();

		void createRenderPass();

		void createGraphicsPipeline();

		void createFramebuffers();

		void createCommandPool();

		void createCommandBuffers();

		void recreateSwapChain();

		void cleanupSwapChain();

		void createSemaphores();

		void updateUniformBuffer();

		void drawFrame();

		VkShaderModule createShaderModule(const std::vector<char>& code);

		std::vector<char> readFile(const std::string& filename);

		VkSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
	};
}
