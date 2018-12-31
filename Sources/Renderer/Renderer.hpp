#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"
#include "Commands/CommandBuffer.hpp"
#include "Swapchain/Swapchain.hpp"
#include "Textures/DepthStencil.hpp"
#include "RenderManager.hpp"
#include "RendererRegister.hpp"
#include "RenderStage.hpp"

namespace acid
{
	class ACID_EXPORT Renderer :
		public Module
	{
	private:
		std::unique_ptr<RenderManager> m_renderManager;

		RendererRegister m_rendererRegister;

		std::vector<std::unique_ptr<RenderStage>> m_renderStages;

		std::unique_ptr<Swapchain> m_swapchain;
		VkFence m_fenceSwapchainImage;
		uint32_t m_activeSwapchainImage;

		VkPipelineCache m_pipelineCache;

		VkSemaphore m_semaphore;
		VkCommandPool m_commandPool;

		std::unique_ptr<CommandBuffer> m_commandBuffer;
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Renderer *Get() { return Engine::Get()->GetModuleManager().Get<Renderer>(); }

		Renderer();

		~Renderer();

		void Update() override;

		void CreateRenderpass(const std::vector<RenderpassCreate> &renderpassCreates);

		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into a image file.
		/// </summary>
		///	<param name="filename"> The file to save the screenshot to. </param>
		void CaptureScreenshot(const std::string &filename);

		/// <summary>
		/// Gets the renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		RenderManager *GetManager() const { return m_renderManager.get(); }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void SetManager(RenderManager *managerRender) { m_renderManager.reset(managerRender); }

		/// <summary>
		/// Gets the renderer register used by the engine. The refister can be used to register/deregister renderers.
		/// </summary>
		/// <returns> The renderer register. </returns>
		RendererRegister &GetRendererRegister() { return m_rendererRegister; }

		RenderStage *GetRenderStage(const uint32_t &index) const;

		Descriptor *GetAttachment(const std::string &name) const;

		Swapchain *GetSwapchain() const { return m_swapchain.get(); }

		VkCommandPool GetCommandPool() const { return m_commandPool; }

		CommandBuffer *GetCommandBuffer() const { return m_commandBuffer.get(); }

		uint32_t GetActiveSwapchainImage() const { return m_activeSwapchainImage; }

		VkPipelineCache GetPipelineCache() const { return m_pipelineCache; }
	private:
		void CreateFences();

		void CreateCommandPool();

		void CreatePipelineCache();

		void RecreatePass(const uint32_t &i);

		bool StartRenderpass(const uint32_t &i);

		void EndRenderpass(const uint32_t &i);

		void NextSubpass();
	};
}
