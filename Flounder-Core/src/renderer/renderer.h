#pragma once

#include "../devices/display.h"
#include "../framework/framework.h"
#include "../shaders/shader.h"
#include "../textures/texture.h"

#include "swapchain/swapchain.h"

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

		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_graphicsPipeline;

		VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;

		VkSemaphore m_imageAvailableSemaphore;
		VkSemaphore m_renderFinishedSemaphore;

		swapchain *m_swapChain;
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
		void createRenderPass();

		void createGraphicsPipeline();

		void createCommandPool();

		void createCommandBuffers();

		void createSemaphores();

		void updateUniformBuffer();

		void drawFrame();

		VkSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	};
}
