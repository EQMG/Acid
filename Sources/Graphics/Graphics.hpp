#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"
#include "Maths/Timer.hpp"
#include "Commands/CommandBuffer.hpp"
#include "Commands/CommandPool.hpp"
#include "Devices/Instance.hpp"
#include "Devices/LogicalDevice.hpp"
#include "Devices/PhysicalDevice.hpp"
#include "Devices/Surface.hpp"
#include "Devices/Window.hpp"
#include "Renderer.hpp"
#include "RenderStage.hpp"

namespace acid
{
/**
 * @brief Module that manages the Vulkan instance, Surface, Window and the renderpass structure.
 */
class ACID_EXPORT Graphics :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Graphics *Get() { return Engine::Get()->GetModule<Graphics>(); }

	Graphics();

	~Graphics();

	void Update() override;

	static std::string StringifyResultVk(const VkResult &result);

	static void CheckVk(const VkResult &result);

	void UpdateSurfaceCapabilities();

	/**
	 * Takes a screenshot of the current image of the display and saves it into a image file.
	 * @param filename The file to save the screenshot to.
	 */
	void CaptureScreenshot(const std::string &filename);

	/**
	 * Checks whether a Render exists or not.
	 * @tparam T The Render type.
	 * @return If the Render has the System.
	 */
	 /*template<typename T>
	 bool HasRender() const
	 {
		 return m_rendererManager->m_renderHolder.Has<T>();
	 }*/

	 /**
	  * Gets a Render.
	  * @tparam T The Render type.
	  * @return The Render.
	  */
	template<typename T>
	T* GetRender() const
	{
		return m_rendererManager->m_renderHolder.Get<T>();
	}

	/**
	 * Adds a Render.
	 * @tparam T The Render type.
	 * @param pipelineStage The Render pipeline stage.
	 * @tparam Args The constructor arg types.
	 * @param args The constructor arguments.
	 */
	template<typename T, typename... Args>
	void AddRender(const Pipeline::Stage& pipelineStage, Args&& ...args)
	{
		m_rendererManager->m_renderHolder.Add<T>(pipelineStage, std::forward(args)...);
	}

	/**
	 * Removes a Render.
	 * @tparam T The Render type.
	 */
	template<typename T>
	void RemoveRender()
	{
		m_rendererManager->m_renderHolder.Remove<T>();
	}

	/**
	 * Gets the current renderer manager.
	 * @return The renderer manager.
	 */
	Renderer *GetManager() const { return m_rendererManager.get(); }

	/**
	 * Sets the current renderer manager to a new renderer manager.
	 * @param managerRender The new renderer manager.
	 */
	void SetManager(Renderer *managerRender) { m_rendererManager.reset(managerRender); }

	RenderStage *GetRenderStage(const uint32_t &index) const;

	void SetRenderStages(std::vector<std::unique_ptr<RenderStage>> renderStages);

	const Descriptor *GetAttachment(const std::string &name) const;

	const Swapchain *GetSwapchain() const { return m_swapchain.get(); }

	const std::shared_ptr<CommandPool> &GetCommandPool(const std::thread::id &threadId = std::this_thread::get_id());

	const VkPipelineCache &GetPipelineCache() const { return m_pipelineCache; }

	const PhysicalDevice *GetPhysicalDevice() const { return m_physicalDevice.get(); }

	const Surface *GetSurface() const { return m_surface.get(); }

	const LogicalDevice *GetLogicalDevice() const { return m_logicalDevice.get(); }

private:
	void CreatePipelineCache();

	void RecreatePass(RenderStage &renderStage);

	void RecreateAttachmentsMap();

	bool StartRenderpass(RenderStage &renderStage);

	void EndRenderpass(RenderStage &renderStage);

	std::unique_ptr<Renderer> m_rendererManager;
	std::vector<std::unique_ptr<RenderStage>> m_renderStages;
	std::map<std::string, const Descriptor *> m_attachments;
	std::unique_ptr<Swapchain> m_swapchain;

	std::map<std::thread::id, std::shared_ptr<CommandPool>> m_commandPools;
	Timer m_timerPurge;

	VkPipelineCache m_pipelineCache;
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
