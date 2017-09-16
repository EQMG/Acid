#pragma once

#include "../devices/Display.hpp"
#include "../engine/Engine.hpp"
#include "../shaders/Shader.hpp"
#include "../textures/Texture.hpp"

#include "buffers/IndexBuffer.hpp"
#include "buffers/VertexBuffer.hpp"
#include "command/CommandPool.hpp"
#include "pass/RenderPass.hpp"
#include "pipelines/Pipeline.hpp"
#include "queue/QueueFamily.hpp"
#include "swapchain/Swapchain.hpp"

#include "IManagerRender.hpp"

namespace Flounder
{
	class Renderer :
		public IModule
	{
	private:
		IManagerRender *m_managerRender;

		RenderPass *m_renderPass;
		Swapchain *m_swapchain;

		CommandPool *m_commandPool;
		CommandPool *m_commandPoolTransfer;

		std::vector<VkCommandBuffer> m_commandBuffers;
		std::vector<VkFence> m_commandBufferFences;

		VkSemaphore m_imageAvailableSemaphore;
		VkSemaphore m_renderFinishedSemaphore;
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
	private:
		void CreateCommandBuffers();

		void CreateSemaphores();
	public:
		void Update() override;

		void PreRendering();

		void BeginReindering();

		void NextSubpass();

		void EndRendering();

		void PostRendering();

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

		VkRenderPass GetRenderPass() const { return m_renderPass->GetRenderPass(); }
	};
}
