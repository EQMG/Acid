#pragma once

#include "../engine/Engine.hpp"

#include "pass/RenderPass.hpp"
#include "pipelines/Pipeline.hpp"
#include "stencils/DepthStencil.hpp"
#include "swapchain/Swapchain.hpp"

#include "IManagerRender.hpp"

namespace Flounder
{
	class Renderer :
		public IModule
	{
	private:
		IManagerRender *m_managerRender;

		Swapchain m_swapchain;
		DepthStencil m_depthStencil;
		RenderPass m_renderPass;
		VkFence m_fenceSwapchainImage;
		uint32_t m_activeSwapchinImage;

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

		void BeginReindering();

		void NextSubpass();

		void EndRendering(std::vector<VkSemaphore> waitSemaphores);

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

		VkRenderPass GetRenderPass() { return m_renderPass.GetRenderPass(); }

		VkCommandPool GetCommandPool() { return m_commandPool; }
	private:
		VkFramebuffer GetActiveFramebuffer() const { return m_swapchain.GetFramebuffers()[m_activeSwapchinImage]; }

		void CreateFences();

		void CreateCommandPool();
	};
}
