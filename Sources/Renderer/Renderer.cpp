#include "Renderer.hpp"

#include <SPIRV/GlslangToSpv.h>
#include "Files/FileSystem.hpp"
#include "Subrender.hpp"

namespace acid
{
Renderer::Renderer() :
	m_rendererManager(nullptr),
	m_swapchain(nullptr),
	m_timerPurge(Time::Seconds(4.0f)),
	m_pipelineCache(VK_NULL_HANDLE),
	m_currentFrame(0),
	m_instance(std::make_unique<Instance>()),
	m_physicalDevice(std::make_unique<PhysicalDevice>(m_instance.get())),
	m_surface(std::make_unique<Surface>(m_instance.get(), m_physicalDevice.get())),
	m_logicalDevice(std::make_unique<LogicalDevice>(m_instance.get(), m_physicalDevice.get(), m_surface.get()))
{
	glslang::InitializeProcess();

	CreatePipelineCache();
}

Renderer::~Renderer()
{
	auto graphicsQueue = m_logicalDevice->GetGraphicsQueue();

	CheckVk(vkQueueWaitIdle(graphicsQueue));

	glslang::FinalizeProcess();

	vkDestroyPipelineCache(*m_logicalDevice, m_pipelineCache, nullptr);

	for (size_t i = 0; i < m_flightFences.size(); i++)
	{
		vkDestroyFence(*m_logicalDevice, m_flightFences[i], nullptr);
		vkDestroySemaphore(*m_logicalDevice, m_renderCompletes[i], nullptr);
		vkDestroySemaphore(*m_logicalDevice, m_presentCompletes[i], nullptr);
	}
}

void Renderer::Update()
{
	if (m_rendererManager == nullptr || Window::Get()->IsIconified())
	{
		return;
	}

	m_rendererManager->Update();
	const auto &stages = m_rendererManager->m_renderHolder.m_stages;

	std::optional<uint32_t> renderpass;
	uint32_t subpass = 0;

	VkResult acquireResult = m_swapchain->AcquireNextImage(m_presentCompletes[m_currentFrame]);

	if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		VkExtent2D displayExtent = { Window::Get()->GetSize().m_x, Window::Get()->GetSize().m_y };
		m_swapchain = std::make_unique<Swapchain>(displayExtent);
		return;
	}

	if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
	{
		return;
	}

	for (auto &[key, renders] : stages)
	{
		if (renderpass != key.first)
		{
			// Ends the previous renderpass.
			if (renderpass)
			{
				EndRenderpass(*GetRenderStage(*renderpass));
			}

			renderpass = key.first;
			subpass = 0;

			// Starts the next renderpass.
			auto renderStage = GetRenderStage(*renderpass);
			renderStage->Update();
			auto startResult = StartRenderpass(*renderStage);

			if (!startResult)
			{
				return;
			}
		}

		auto renderStage = GetRenderStage(*renderpass);

		// Changes the subpass.
		if (subpass != key.second)
		{
			uint32_t difference = key.second - subpass;

			if (subpass == static_cast<uint32_t>(renderStage->GetSubpasses().size() - 1))
			{
				difference -= 1;
			}

			for (uint32_t d = 0; d < difference; d++)
			{
				vkCmdNextSubpass(*m_commandBuffers[m_swapchain->GetActiveImageIndex()], VK_SUBPASS_CONTENTS_INLINE);
			}

			subpass = key.second;
		}

		// Renders subpass render pipeline.
		for (auto &render : renders)
		{
			if (render->IsEnabled())
			{
				render->Render(*m_commandBuffers[m_swapchain->GetActiveImageIndex()]);
			}
		}
	}

	// Ends the last renderpass.
	if (renderpass)
	{
		auto renderStage = GetRenderStage(*renderpass);

		if (renderStage != nullptr)
		{
			EndRenderpass(*renderStage);
		}
	}

	// Purges unused command pools.
	if (m_timerPurge.IsPassedTime())
	{
		m_timerPurge.ResetStartTime();

		for (auto it = m_commandPools.begin(); it != m_commandPools.end();)
		{
			if ((*it).second.use_count() <= 1)
			{
				it = m_commandPools.erase(it);
				continue;
			}

			++it;
		}
	}
}

std::string Renderer::StringifyResultVk(const VkResult &result)
{
	switch (result)
	{
	case VK_SUCCESS:
		return "Success";
	case VK_NOT_READY:
		return "A fence or query has not yet completed";
	case VK_TIMEOUT:
		return "A wait operation has not completed in the specified time";
	case VK_EVENT_SET:
		return "An event is signaled";
	case VK_EVENT_RESET:
		return "An event is unsignaled";
	case VK_INCOMPLETE:
		return "A return array was too small for the result";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "A host memory allocation has failed";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "A device memory allocation has failed";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "Initialization of an object could not be completed for implementation-specific reasons";
	case VK_ERROR_DEVICE_LOST:
		return "The logical or physical device has been lost";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "Mapping of a memory object has failed";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "A requested layer is not present or could not be loaded";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "A requested extension is not supported";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "A requested feature is not supported";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "Too many objects of the type have already been created";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "A requested format is not supported on this device";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "A surface is no longer available";
		//case VK_ERROR_OUT_OF_POOL_MEMORY:
		//return "A allocation failed due to having no more space in the descriptor pool";
	case VK_SUBOPTIMAL_KHR:
		return "A swapchain no longer matches the surface properties exactly, but can still be used";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "A surface has changed in such a way that it is no longer compatible with the swapchain";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "The display used by a swapchain does not use the same presentable image layout";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "A validation layer found an error";
	default:
		return "Unknown Vulkan error";
	}
}

void Renderer::CheckVk(const VkResult &result)
{
	if (result >= 0)
	{
		return;
	}

	std::string failure = StringifyResultVk(result);

	Log::Error("Vulkan error: %s, %i\n", failure.c_str(), result);
	Log::Popup("Vulkan Error", failure);
	throw std::runtime_error("Vulkan error: " + failure);
}

void Renderer::UpdateSurfaceCapabilities()
{
	CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*m_physicalDevice, *m_surface, &m_surface->m_capabilities));
}

void Renderer::CaptureScreenshot(const std::string &filename)
{
#if defined(ACID_VERBOSE)
	auto debugStart = Engine::GetTime();
#endif

	auto extent = Window::Get()->GetSize();

	VkImage dstImage;
	VkDeviceMemory dstImageMemory;
	bool supportsBlit = Image::CopyImage(m_swapchain->GetActiveImage(), dstImage, dstImageMemory, m_surface->GetFormat().format, { extent.m_x, extent.m_y, 1 },
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 0, 0);

	// Get layout of the image (including row pitch).
	VkImageSubresource imageSubresource = {};
	imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageSubresource.mipLevel = 0;
	imageSubresource.arrayLayer = 0;

	VkSubresourceLayout dstSubresourceLayout;
	vkGetImageSubresourceLayout(*m_logicalDevice, dstImage, &imageSubresource, &dstSubresourceLayout);

	auto pixels = std::make_unique<uint8_t[]>(dstSubresourceLayout.size);

	void *data;
	vkMapMemory(*m_logicalDevice, dstImageMemory, dstSubresourceLayout.offset, dstSubresourceLayout.size, 0, &data);
	std::memcpy(pixels.get(), data, static_cast<size_t>(dstSubresourceLayout.size));
	vkUnmapMemory(*m_logicalDevice, dstImageMemory);

	// Frees temp image and memory.
	vkFreeMemory(*m_logicalDevice, dstImageMemory, nullptr);
	vkDestroyImage(*m_logicalDevice, dstImage, nullptr);

	// Creates the screenshot image file and writes to it.
	FileSystem::ClearFile(filename);
	Image::WritePixels(filename, pixels.get(), extent);

#if defined(ACID_VERBOSE)
	auto debugEnd = Engine::GetTime();
	Log::Out("Screenshot '%s' saved in %.3fms\n", filename.c_str(), (debugEnd - debugStart).AsMilliseconds<float>());
#endif
}

RenderStage *Renderer::GetRenderStage(const uint32_t &index) const
{
	if (m_renderStages.empty() || m_renderStages.size() < index)
	{
		return nullptr;
	}

	return m_renderStages.at(index).get();
}

void Renderer::SetRenderStages(std::vector<std::unique_ptr<RenderStage>> renderStages)
{
	VkExtent2D displayExtent = { Window::Get()->GetSize().m_x, Window::Get()->GetSize().m_y };

	m_renderStages = std::move(renderStages);
	m_swapchain = std::make_unique<Swapchain>(displayExtent);

	if (m_flightFences.size() != m_swapchain->GetImageCount())
	{
		for (size_t i = 0; i < m_flightFences.size(); i++)
		{
			vkDestroyFence(*m_logicalDevice, m_flightFences[i], nullptr);
			vkDestroySemaphore(*m_logicalDevice, m_renderCompletes[i], nullptr);
			vkDestroySemaphore(*m_logicalDevice, m_presentCompletes[i], nullptr);
		}

		m_presentCompletes.resize(m_swapchain->GetImageCount());
		m_renderCompletes.resize(m_swapchain->GetImageCount());
		m_flightFences.resize(m_swapchain->GetImageCount());
		m_commandBuffers.resize(m_swapchain->GetImageCount());

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < m_flightFences.size(); i++)
		{
			CheckVk(vkCreateSemaphore(*m_logicalDevice, &semaphoreCreateInfo, nullptr, &m_presentCompletes[i]));

			CheckVk(vkCreateSemaphore(*m_logicalDevice, &semaphoreCreateInfo, nullptr, &m_renderCompletes[i]));

			CheckVk(vkCreateFence(*m_logicalDevice, &fenceCreateInfo, nullptr, &m_flightFences[i]));

			m_commandBuffers[i] = std::make_unique<CommandBuffer>(false);
		}
	}

	for (const auto &renderStage : m_renderStages)
	{
		renderStage->Rebuild(*m_swapchain);
	}

	RecreateAttachmentsMap();
}

const Descriptor *Renderer::GetAttachment(const std::string &name) const
{
	auto it = m_attachments.find(name);

	if (it == m_attachments.end())
	{
		return nullptr;
	}

	return it->second;
}

const std::shared_ptr<CommandPool> &Renderer::GetCommandPool(const std::thread::id &threadId)
{
	auto it = m_commandPools.find(threadId);

	if (it != m_commandPools.end())
	{
		return it->second;
	}

	m_commandPools.emplace(threadId, std::make_shared<CommandPool>(threadId));
	return m_commandPools.find(threadId)->second; // TODO: Cleanup.
}

void Renderer::CreatePipelineCache()
{
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	CheckVk(vkCreatePipelineCache(*m_logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
}

void Renderer::RecreatePass(RenderStage &renderStage)
{
	auto graphicsQueue = m_logicalDevice->GetGraphicsQueue();

	VkExtent2D displayExtent = { Window::Get()->GetSize().m_x, Window::Get()->GetSize().m_y };

	CheckVk(vkQueueWaitIdle(graphicsQueue));

	if (renderStage.HasSwapchain() && !m_swapchain->IsSameExtent(displayExtent))
	{
#if defined(ACID_VERBOSE)
		Log::Out("Resizing swapchain from (%i, %i) to (%i, %i)\n", m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, displayExtent.width, displayExtent.height);
#endif
		m_swapchain = std::make_unique<Swapchain>(displayExtent);
	}

	renderStage.Rebuild(*m_swapchain);
	RecreateAttachmentsMap(); // TODO: Maybe not recreate on a single change.
}

void Renderer::RecreateAttachmentsMap()
{
	m_attachments.clear();

	for (const auto &renderStage : m_renderStages)
	{
		m_attachments.insert(renderStage->m_descriptors.begin(), renderStage->m_descriptors.end());
	}
}

bool Renderer::StartRenderpass(RenderStage &renderStage)
{
	if (renderStage.IsOutOfDate())
	{
		RecreatePass(renderStage);
		return false;
	}

	if (!m_commandBuffers[m_swapchain->GetActiveImageIndex()]->IsRunning())
	{
		CheckVk(vkWaitForFences(*m_logicalDevice, 1, &m_flightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max()));
		m_commandBuffers[m_swapchain->GetActiveImageIndex()]->Begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
	}

	VkRect2D renderArea = {};
	renderArea.offset = { renderStage.GetRenderArea().GetOffset().m_x, renderStage.GetRenderArea().GetOffset().m_y };
	renderArea.extent = { renderStage.GetRenderArea().GetExtent().m_x, renderStage.GetRenderArea().GetExtent().m_y };

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(renderArea.extent.width);
	viewport.height = static_cast<float>(renderArea.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(*m_commandBuffers[m_swapchain->GetActiveImageIndex()], 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = renderArea.offset;
	scissor.extent = renderArea.extent;
	vkCmdSetScissor(*m_commandBuffers[m_swapchain->GetActiveImageIndex()], 0, 1, &scissor);

	auto clearValues = renderStage.GetClearValues();

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderStage.GetRenderpass()->GetRenderpass();
	renderPassBeginInfo.framebuffer = renderStage.GetActiveFramebuffer(m_swapchain->GetActiveImageIndex());
	renderPassBeginInfo.renderArea = renderArea;
	renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassBeginInfo.pClearValues = clearValues.data();
	vkCmdBeginRenderPass(*m_commandBuffers[m_swapchain->GetActiveImageIndex()], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	return true;
}

void Renderer::EndRenderpass(RenderStage &renderStage)
{
	auto presentQueue = m_logicalDevice->GetPresentQueue();

	vkCmdEndRenderPass(*m_commandBuffers[m_swapchain->GetActiveImageIndex()]);

	if (!renderStage.HasSwapchain())
	{
		return;
	}

	m_commandBuffers[m_swapchain->GetActiveImageIndex()]->End();
	m_commandBuffers[m_swapchain->GetActiveImageIndex()]->Submit(m_presentCompletes[m_currentFrame], m_renderCompletes[m_currentFrame], m_flightFences[m_currentFrame]);
	VkResult presentResult = m_swapchain->QueuePresent(presentQueue, m_renderCompletes[m_currentFrame]);

	if (!(presentResult == VK_SUCCESS || presentResult == VK_SUBOPTIMAL_KHR))
	{
		if (presentResult == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreatePass(renderStage);
			return;
		}
		CheckVk(presentResult);
	}

	m_currentFrame = (m_currentFrame + 1) % m_swapchain->GetImageCount();
}
}
