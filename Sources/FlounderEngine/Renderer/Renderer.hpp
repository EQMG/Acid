#pragma once

#include "../Engine/Engine.hpp"
#include "Pass/RenderPass.hpp"
#include "Stencils/DepthStencil.hpp"
#include "Swapchain/Swapchain.hpp"
#include "Swapchain/Framebuffers.hpp"
#include "IManagerRender.hpp"

namespace Flounder
{
	class Renderer :
		public IModule
	{
	private:
		IManagerRender *m_managerRender;

		Swapchain *m_swapchain;
		DepthStencil *m_depthStencil;
		RenderPass *m_renderPass;
		Framebuffers *m_framebuffers;
		VkFence m_fenceSwapchainImage;
		uint32_t m_activeSwapchainImage;

		VkPipelineCache m_pipelineCache;

		VkSemaphore m_semaphore;
		VkCommandPool m_commandPool;
		VkCommandBuffer m_commandBuffer;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Renderer *Get()
		{
			return reinterpret_cast<Renderer *>(Engine::Get()->GetModule("renderer"));
		}

		/// <summary>
		/// Creates a new renderer module.
		/// </summary>
		Renderer();

		/// <summary>
		/// Deconstructor for the renderer module.
		/// </summary>
		~Renderer();

	public:
		void Update() override;

		/// <summary>
		/// Starts a renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		/// <returns> VK_SUCCESS on success. </returns>
		VkResult StartRenderpass(const VkCommandBuffer &commandBuffer);

		/// <summary>
		/// Starts the next render subpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		void NextSubpass(const VkCommandBuffer &commandBuffer);

		/// <summary>
		/// Ends the renderpass.
		/// </summary>
		/// <param name="commandBuffer"> The command buffer to use. </param>
		void EndRenderpass(const VkCommandBuffer &commandBuffer);

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		IManagerRender *GetManager() const { return m_managerRender; }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void SetManager(IManagerRender *managerRender) { m_managerRender = managerRender; }

		Swapchain *GetSwapchain() const { return m_swapchain; }

		DepthStencil *GetDepthStencil() const { return m_depthStencil; }

		VkRenderPass GetRenderPass() const { return m_renderPass->GetRenderPass(); }

		VkCommandPool GetCommandPool() const { return m_commandPool; }

		VkCommandBuffer GetCommandBuffer() const { return m_commandBuffer; }

		VkPipelineCache GetPipelineCache() const { return m_pipelineCache; }

		static VkCommandBuffer BeginSingleTimeCommands();

		static void EndSingleTimeCommands(const VkCommandBuffer &commandBuffer);

	private:
		VkFramebuffer GetActiveFramebuffer() const { return m_framebuffers->GetFramebuffers()[m_activeSwapchainImage]; }

		void CreateFences();

		void CreateCommandPool();

		void CreatePipelineCache();

		void RecreateSwapchain();
	};
}
