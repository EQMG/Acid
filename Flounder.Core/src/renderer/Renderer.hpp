#pragma once

#include "../engine/Engine.hpp"
#include "pass/RenderPass.hpp"
#include "stencils/DepthStencil.hpp"
#include "swapchain/Swapchain.hpp"
#include "swapchain/Framebuffers.hpp"
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
		uint32_t m_activeSwapchinImage;

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
			return static_cast<Renderer*>(Engine::Get()->GetModule("renderer"));
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

		VkRenderPass GetRenderPass() const { return m_renderPass->GetRenderPass(); }

		VkCommandPool GetCommandPool() const { return m_commandPool; }

		VkCommandBuffer GetCommandBuffer() const { return m_commandBuffer; }

		VkPipelineCache GetPipelineCache() const { return m_pipelineCache; }

		static VkCommandBuffer BeginSingleTimeCommands();

		static void EndSingleTimeCommands(const VkCommandBuffer &commandBuffer);
	private:
		VkFramebuffer GetActiveFramebuffer() const { return m_framebuffers->GetFramebuffers()[m_activeSwapchinImage]; }

		void CreateFences();

		void CreateCommandPool();

		void CreatePipelineCache();

		void RecreateSwapchain();

		VkResult BeginReindering();

		void RunRenderPass();

		void EndRendering(std::vector<VkSemaphore> waitSemaphores);
	};
}
