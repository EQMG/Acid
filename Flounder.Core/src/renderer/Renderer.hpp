#pragma once

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "../shaders/Shader.hpp"
#include "../textures/Texture.hpp"

#include "buffers/IndexBuffer.hpp"
#include "buffers/VertexBuffer.hpp"
#include "command/CommandPool.hpp"
#include "queue/QueueFamily.hpp"
#include "swapchain/Swapchain.hpp"

#include "IManagerRender.hpp"

namespace Flounder
{
	class RendererTest;

	class Renderer :
		public IModule
	{
	private:
		IManagerRender *m_managerRender;

		VkRenderPass m_renderPass;
		VkPipelineLayout m_pipelineLayout;
		VkPipeline m_graphicsPipeline;

		Swapchain *m_swapchain;

		CommandPool *m_commandPool;
		CommandPool *m_commandPoolTransfer;

		std::vector<VkCommandBuffer> m_commandBuffers;

		VkSemaphore m_imageAvailableSemaphore;
		VkSemaphore m_renderFinishedSemaphore;

		RendererTest *m_rendererTest;

		int lastWidth;
		int lastHeight;
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

		void Update() override;

		void RecreateSwapChain();

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

		std::vector<VkCommandBuffer> GetVkCommandBuffers() const { return m_commandBuffers; }

		CommandPool *GetCommandPoolTransfer() const { return m_commandPoolTransfer; }

	private:
		void CreateRenderPass();

		void CreateGraphicsPipeline();

		void CreateCommandBuffers();

		void CreateSemaphores();

		void DrawFrame();

		void CleanupSwapChain();
	};
}
