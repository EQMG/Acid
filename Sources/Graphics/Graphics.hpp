#pragma once

#include <vulkan/vulkan.h>
#include "Engine/Engine.hpp"
#include "Commands/CommandBuffer.hpp"
#include "Commands/CommandPool.hpp"
#include "Devices/Instance.hpp"
#include "Devices/LogicalDevice.hpp"
#include "Devices/PhysicalDevice.hpp"
#include "Devices/Surface.hpp"
#include "Renderer.hpp"
#include "RenderStage.hpp"
#include "SubrenderHolder.hpp"

namespace acid {
/**
 * @brief Module that manages the Vulkan instance, Surface, Window and the renderpass structure.
 */
class ACID_EXPORT
Graphics
:
public
Module
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
	 * @param filename The file to save the screenshot as.
	 */
	void CaptureScreenshot(const std::filesystem::path &filename) const;

	/**
	 * Checks whether a Subrender exists or not.
	 * @tparam T The Subrender type.
	 * @return If the Subrender has the System.
	 */
	 /*template<typename T>
	 bool HasSubrender() const
	 {
		 return m_subrenderHolder.Has<T>();
	 }*/

	 /**
	  * Gets a Subrender.
	  * @tparam T The Subrender type.
	  * @return The Subrender.
	  */
	template<typename T>
	T* GetSubrender() const
	{
		return m_subrenderHolder.Get<T>();
	}

	/**
	 * Adds a Subrender.
	 * @tparam T The Subrender type.
	 * @param pipelineStage The Subrender pipeline stage.
	 * @tparam Args The constructor arg types.
	 * @param args The constructor arguments.
	 */
	template<typename T, typename... Args>
	void AddSubrender(const Pipeline::Stage& pipelineStage, Args &&...args)
	{
		m_subrenderHolder.Add<T>(pipelineStage, std::make_unique<T>(pipelineStage, std::forward<Args>(args)...));
	}

	/**
	 * Removes a Subrender.
	 * @tparam T The Subrender type.
	 */
	template<typename T>
	void RemoveSubrender()
	{
		m_subrenderHolder.Remove<T>();
	}

	/**
	 * Clears all Subrenders.
	 */
	void ClearSubrenders()
	{
		m_subrenderHolder.Clear();
	}

	/**
	 * Gets the current renderer.
	 * @return The renderer.
	 */
	Renderer *GetRenderer() const { return m_renderer.get(); }

	/**
	 * Sets the current renderer to a new renderer.
	 * @param renderer The new renderer.
	 */
	void SetRenderer(std::unique_ptr<Renderer> &&renderer) { m_renderer = std::move(renderer); }

	RenderStage *GetRenderStage(uint32_t index) const;

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

	std::unique_ptr<Renderer> m_renderer;
	SubrenderHolder m_subrenderHolder;
	std::vector<std::unique_ptr<RenderStage>> m_renderStages;
	std::map<std::string, const Descriptor *> m_attachments;
	std::unique_ptr<Swapchain> m_swapchain;

	std::map<std::thread::id, std::shared_ptr<CommandPool>> m_commandPools;
	ElapsedTime m_elapsedPurge;

	VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;
	std::vector<VkSemaphore> m_presentCompletes;
	std::vector<VkSemaphore> m_renderCompletes;
	std::vector<VkFence> m_flightFences;
	std::size_t m_currentFrame = 0;

	std::vector<std::unique_ptr<CommandBuffer>> m_commandBuffers;

	std::unique_ptr<Instance> m_instance;
	std::unique_ptr<PhysicalDevice> m_physicalDevice;
	std::unique_ptr<Surface> m_surface;
	std::unique_ptr<LogicalDevice> m_logicalDevice;
};
}
