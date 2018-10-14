#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"
#include "Commands/CommandBuffer.hpp"
#include "Swapchain/DepthStencil.hpp"
#include "Swapchain/Swapchain.hpp"
#include "IManagerRender.hpp"
#include "RendererRegister.hpp"
#include "RenderStage.hpp"

namespace acid
{
	class ACID_EXPORT Renderer :
		public IModule
	{
	private:
		std::unique_ptr<IManagerRender> m_managerRender;

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
		static Renderer *Get() { return Engine::Get()->GetModule<Renderer>(); }

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
		IManagerRender *GetManager() const { return m_managerRender.get(); }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void SetManager(IManagerRender *managerRender) { m_managerRender.reset(managerRender); }

		/// <summary>
		/// Gets a renderer instance by type from this register.
		/// </summary>
		/// <param name="T"> The renderer type to find. </param>
		/// <param name="allowDisabled"> If disabled renderers will be returned. </param>
		/// <returns> The found renderer. </returns>
		template<typename T>
		T *GetRenderer(const bool &allowDisabled = false) { return m_rendererRegister.GetRenderer<T>(allowDisabled); }

		/// <summary>
		/// Adds a renderer to this register.
		/// </summary>
		/// <param name="renderer"> The renderer to add. </param>
		/// <returns> The added renderer. </returns>
		IRenderer *AddRenderer(IRenderer *renderer) { return m_rendererRegister.AddRenderer(renderer); }

		/// <summary>
		/// Creates a renderer by type to be added this register.
		/// </summary>
		/// <param name="T"> The type of renderer to add. </param>
		/// <param name="args"> The type constructor arguments. </param>
		/// <returns> The added renderer. </returns>
		template<typename T, typename... Args>
		T *AddRenderer(Args &&... args) { return m_rendererRegister.AddRenderer<T>(std::forward<Args>(args)...); }

		/// <summary>
		/// Removes a renderer from this register.
		/// </summary>
		/// <param name="renderer"> The renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		bool RemoveRenderer(IRenderer *renderer) { return m_rendererRegister.RemoveRenderer(renderer); }

		/// <summary>
		/// Removes a renderer by type from this register.
		/// </summary>
		/// <param name="T"> The type of renderer to remove. </param>
		/// <returns> If the renderer was removed. </returns>
		template<typename T>
		bool RemoveRenderer() { return m_rendererRegister.RemoveRenderer<T>(); }

		RenderStage *GetRenderStage(const uint32_t &index) const;

		IDescriptor *GetAttachment(const std::string &name) const;

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
