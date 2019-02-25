#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"
#include "Commands/CommandBuffer.hpp"
#include "Devices/Instance.hpp"
#include "Devices/LogicalDevice.hpp"
#include "Devices/PhysicalDevice.hpp"
#include "Devices/Surface.hpp"
#include "Devices/Window.hpp"
#include "RenderManager.hpp"
#include "RenderStage.hpp"

namespace acid
{
	class ACID_EXPORT Renderer :
		public Module
	{
	public:
		/// <summary>
		/// Gets this engine instance.
		/// </summary>
		/// <returns> The current module instance. </returns>
		static Renderer *Get() { return Engine::Get()->GetModuleManager().Get<Renderer>(); }

		Renderer();

		~Renderer();

		void Update() override;

		static std::string StringifyResultVk(const VkResult &result);

		static void CheckVk(const VkResult &result);

		void UpdateSurfaceCapabilities();

		/// <summary>
		/// Takes a screenshot of the current image of the display and saves it into a image file.
		/// </summary>
		///	<param name="filename"> The file to save the screenshot to. </param>
		void CaptureScreenshot(const std::string &filename);

		/// <summary>
		/// Gets the current renderer manager.
		/// </summary>
		/// <returns> The renderer manager. </returns>
		RenderManager *GetManager() const { return m_renderManager.get(); }

		/// <summary>
		/// Sets the current renderer manager to a new renderer manager.
		/// </summary>
		/// <param name="rendererMaster"> The new renderer manager. </param>
		void SetManager(RenderManager *managerRender) { m_renderManager.reset(managerRender); }

		RenderStage *GetRenderStage(const uint32_t &index) const;

		void SetRenderStages(const std::vector<RenderStage *> &renderStages);

		const Descriptor *GetAttachment(const std::string &name) const;

		const Swapchain *GetSwapchain() const { return m_swapchain.get(); }

		const VkCommandPool &GetCommandPool() const { return m_commandPool; }

		const VkPipelineCache &GetPipelineCache() const { return m_pipelineCache; }

		const PhysicalDevice *GetPhysicalDevice() const { return m_physicalDevice.get(); }

		const Surface *GetSurface() const { return m_surface.get(); }

		const LogicalDevice *GetLogicalDevice() const { return m_logicalDevice.get(); }
	private:
		void CreateCommandPool();

		void CreatePipelineCache();

		void RecreatePass(RenderStage &renderStage);

		void RecreateAttachmentsMap();

		bool StartRenderpass(RenderStage &renderStage);

		void EndRenderpass(RenderStage &renderStage);

		std::unique_ptr<RenderManager> m_renderManager;
		std::vector<std::unique_ptr<RenderStage>> m_renderStages;
		std::map<std::string, const Descriptor *> m_attachments;
		std::unique_ptr<Swapchain> m_swapchain;

		VkPipelineCache m_pipelineCache;
		VkCommandPool m_commandPool;
		std::vector<VkSemaphore> m_presentCompletes;
		std::vector<VkSemaphore> m_renderCompletes;
		std::vector<VkFence> m_flightFences;
		size_t m_currentFrame;

		std::vector<std::unique_ptr<CommandBuffer>> m_commandBuffers;

		std::unique_ptr<Instance> m_instance;
		std::unique_ptr<PhysicalDevice> m_physicalDevice;
		std::unique_ptr<Surface> m_surface;
		std::unique_ptr<LogicalDevice> m_logicalDevice;
	};
}
