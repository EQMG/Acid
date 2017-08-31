#pragma once

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "../shaders/shader.hpp"
#include "../textures/texture.hpp"

#include "swapchain/swapchain.hpp"

#include "imanagerrender.hpp"

namespace Flounder
{
	class renderer :
		public IModule
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
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static renderer *get()
		{
			return static_cast<renderer*>(Engine::Get()->GetModule("renderer"));
		}

		/// <summary>
		/// Creates a new renderer module.
		/// </summary>
		renderer();

		/// <summary>
		/// Deconstructor for the renderer module.
		/// </summary>
		~renderer();

		void Update() override;

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		imanagerrender *getManager() { return m_managerRender; }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void setManager(imanagerrender *managerRender) { m_managerRender = managerRender; }

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
