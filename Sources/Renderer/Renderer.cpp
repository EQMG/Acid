#include "Renderer.hpp"

#include <cassert>
#include <SPIRV/GlslangToSpv.h>
#include "Renderer/Renderer.hpp"
#include "Files/FileSystem.hpp"
#include "RenderPipeline.hpp"

namespace acid
{
	static const int MAX_FRAMES_IN_FLIGHT = 2; // TODO: Get from on runtime m_swapchain.m_imageCount

	Renderer::Renderer() :
		m_renderManager(nullptr),
		m_renderStages(std::vector<std::unique_ptr<RenderStage>>()),
		m_swapchain(nullptr),
		m_pipelineCache(VK_NULL_HANDLE),
		m_commandPool(VK_NULL_HANDLE),
		m_presentCompletes(std::vector<VkSemaphore>()),
		m_renderCompletes(std::vector<VkSemaphore>()),
		m_flightFences(std::vector<VkFence>()),
		m_currentFrame(0),
		m_commandBuffers(std::vector<std::unique_ptr<CommandBuffer>>()),
		m_instance(std::make_unique<Instance>()),
		m_physicalDevice(std::make_unique<PhysicalDevice>(m_instance.get())),
		m_surface(std::make_unique<Surface>(m_instance.get(), m_physicalDevice.get())),
		m_logicalDevice(std::make_unique<LogicalDevice>(m_instance.get(), m_physicalDevice.get(), m_surface.get()))
	{
		glslang::InitializeProcess();

		CreateCommandPool();
		CreatePipelineCache();
	}

	Renderer::~Renderer()
	{
		auto graphicsQueue = m_logicalDevice->GetGraphicsQueue();

		Renderer::CheckVk(vkQueueWaitIdle(graphicsQueue));

		glslang::FinalizeProcess();

		vkDestroyPipelineCache(m_logicalDevice->GetLogicalDevice(), m_pipelineCache, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroyFence(m_logicalDevice->GetLogicalDevice(), m_flightFences[i], nullptr);
			vkDestroySemaphore(m_logicalDevice->GetLogicalDevice(), m_renderCompletes[i], nullptr);
			vkDestroySemaphore(m_logicalDevice->GetLogicalDevice(), m_presentCompletes[i], nullptr);
		}

		vkDestroyCommandPool(m_logicalDevice->GetLogicalDevice(), m_commandPool, nullptr);
	}

	void Renderer::Update()
	{
		if (m_renderManager == nullptr || Window::Get()->IsIconified())
		{
			return;
		}

		if (!m_renderManager->m_started)
		{
			m_renderManager->GetRendererContainer().Clear();
			CreateRenderpass(m_renderManager->m_renderpassCreates);
			m_renderManager->Start();
			m_renderManager->m_started = true;
		}

		m_renderManager->Update();

		auto &stages = m_renderManager->GetRendererContainer().GetStages();

		if (stages.empty())
		{
			return;
		}

		std::optional<uint32_t> renderpass = {};
		uint32_t subpass = 0;

		VkResult acquireResult = m_swapchain->AcquireNextImage(m_presentCompletes[m_currentFrame]);

		if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
		{
			return;
		}

		if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
		{
			return;
		}

		for (auto &[key, renderPipelines] : stages)
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

				if (subpass == renderStage->SubpassCount() - 1)
				{
					difference -= 1;
				}

				for (uint32_t d = 0; d < difference; d++)
				{
					vkCmdNextSubpass(m_commandBuffers[m_swapchain->GetActiveImageIndex()]->GetCommandBuffer(), VK_SUBPASS_CONTENTS_INLINE);
				}

				subpass = key.second;
			}

			// Renders subpass render pipeline.
			for (auto &renderPipeline : renderPipelines)
			{
				if (!renderPipeline->IsEnabled())
				{
					continue;
				}

				renderPipeline->Render(*m_commandBuffers[m_swapchain->GetActiveImageIndex()]);
			}
		}

		// Ends the last renderpass.
		if (renderpass)
		{
			EndRenderpass(*GetRenderStage(*renderpass));
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
		//	case VK_ERROR_OUT_OF_POOL_MEMORY:
		//		return "A allocation failed due to having no more space in the descriptor pool";
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
		assert(false && "Vulkan error!");
	}

	void Renderer::UpdateSurfaceCapabilities()
	{
		Renderer::CheckVk(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice->GetPhysicalDevice(),
			m_surface->GetSurface(), &m_surface->m_capabilities));
	}

	void Renderer::CaptureScreenshot(const std::string &filename)
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		auto width = Window::Get()->GetWidth();
		auto height = Window::Get()->GetHeight();

		VkImage srcImage = m_swapchain->GetActiveImage();
		VkImage dstImage;
		VkDeviceMemory dstImageMemory;
		bool supportsBlit = Texture::CopyImage(srcImage, dstImage, dstImageMemory, width, height, true, 0, 1);

		// Get layout of the image (including row pitch).
		VkImageSubresource imageSubresource = {};
		imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		VkSubresourceLayout subresourceLayout;
		vkGetImageSubresourceLayout(m_logicalDevice->GetLogicalDevice(), dstImage, &imageSubresource, &subresourceLayout);

		// Creates the screenshot image file.
		FileSystem::Create(filename);

		// Map image memory so we can start copying from it.
		char *data;
		vkMapMemory(m_logicalDevice->GetLogicalDevice(), dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void **) &data);
		data += subresourceLayout.offset;

		// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'll have to manually swizzle color components
		bool colourSwizzle = false;

		// Check if source is BGR.
		if (!supportsBlit)
		{
			std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
			colourSwizzle = std::find(formatsBGR.begin(), formatsBGR.end(), m_surface->GetFormat().format) != formatsBGR.end();
		}

		std::unique_ptr<uint8_t[]> pixels((uint8_t*)malloc(subresourceLayout.size));

		if (colourSwizzle)
		{
			for (uint32_t i = 0; i < width * height; i++)
			{
				auto pixelOffset = (uint8_t *) data + (i * 4);
				pixels.get()[i * 4] = pixelOffset[2];
				pixels.get()[i * 4 + 1] = pixelOffset[1];
				pixels.get()[i * 4 + 2] = pixelOffset[0];
				pixels.get()[i * 4 + 3] = pixelOffset[3];
			}
		}
		else
		{
			memcpy(pixels.get(), (uint8_t *) data, subresourceLayout.size);
		}

		// Writes the image.
		Texture::WritePixels(filename, pixels.get(), width, height, 4);

		// Clean up resources.
		vkUnmapMemory(m_logicalDevice->GetLogicalDevice(), dstImageMemory);
		vkFreeMemory(m_logicalDevice->GetLogicalDevice(), dstImageMemory, nullptr);
		vkDestroyImage(m_logicalDevice->GetLogicalDevice(), dstImage, nullptr);

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("Screenshot '%s' saved in %ims\n", filename.c_str(), (debugEnd - debugStart).AsMilliseconds());
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

	const Descriptor *Renderer::GetAttachment(const std::string &name) const
	{
		for (const auto &renderStage : m_renderStages) // TODO: Generate a map on creation.
		{
			auto attachment = renderStage->GetAttachment(name);

			if (attachment != nullptr)
			{
				return attachment;
			}
		}

		return nullptr;
	}

	void Renderer::CreateCommandPool()
	{
		auto graphicsFamily = m_logicalDevice->GetGraphicsFamily();

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = graphicsFamily;
		Renderer::CheckVk(vkCreateCommandPool(m_logicalDevice->GetLogicalDevice(), &commandPoolCreateInfo, nullptr, &m_commandPool));

		m_presentCompletes.resize(MAX_FRAMES_IN_FLIGHT);
		m_renderCompletes.resize(MAX_FRAMES_IN_FLIGHT);
		m_flightFences.resize(MAX_FRAMES_IN_FLIGHT);
		m_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			Renderer::CheckVk(vkCreateSemaphore(m_logicalDevice->GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_presentCompletes[i]));

			Renderer::CheckVk(vkCreateSemaphore(m_logicalDevice->GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_renderCompletes[i]));

			Renderer::CheckVk(vkCreateFence(m_logicalDevice->GetLogicalDevice(), &fenceCreateInfo, nullptr, &m_flightFences[i]));

			m_commandBuffers[i] = std::make_unique<CommandBuffer>(false);
		}
	}

	void Renderer::CreatePipelineCache()
	{
		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		Renderer::CheckVk(vkCreatePipelineCache(m_logicalDevice->GetLogicalDevice(), &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
	}

	void Renderer::CreateRenderpass(const std::vector<RenderpassCreate> &renderpassCreates)
	{
		VkExtent2D displayExtent = {
			Window::Get()->GetWidth(),
			Window::Get()->GetHeight()
		};

		m_renderStages.clear();
		m_swapchain = std::make_unique<Swapchain>(displayExtent);

		for (const auto &renderpassCreate : renderpassCreates)
		{
			m_renderStages.emplace_back(std::make_unique<RenderStage>(static_cast<uint32_t>(m_renderStages.size()),
				renderpassCreate, *m_swapchain));
		}
	}

	void Renderer::RecreatePass(RenderStage &renderStage)
	{
		auto graphicsQueue = m_logicalDevice->GetGraphicsQueue();

		VkExtent2D displayExtent = {
			Window::Get()->GetWidth(),
			Window::Get()->GetHeight()
		};

		Renderer::CheckVk(vkQueueWaitIdle(graphicsQueue));

		if (renderStage.HasSwapchain() && !m_swapchain->IsSameExtent(displayExtent))
		{
#if defined(ACID_VERBOSE)
			Log::Out("Resizing swapchain from (%i, %i) to (%i, %i)\n", m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, displayExtent.width, displayExtent.height);
#endif
			m_swapchain = std::make_unique<Swapchain>(displayExtent);
		}

		renderStage.Rebuild(*m_swapchain);
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
			Renderer::CheckVk(vkWaitForFences(m_logicalDevice->GetLogicalDevice(), 1, &m_flightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max()));
			m_commandBuffers[m_swapchain->GetActiveImageIndex()]->Begin(VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
		}

		VkRect2D renderArea = {};
		renderArea.offset = { 0, 0 };
		renderArea.extent = {
			renderStage.GetWidth(),
			renderStage.GetHeight()
		};

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(renderArea.extent.width);
		viewport.height = static_cast<float>(renderArea.extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_commandBuffers[m_swapchain->GetActiveImageIndex()]->GetCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = renderArea.extent;
		vkCmdSetScissor(m_commandBuffers[m_swapchain->GetActiveImageIndex()]->GetCommandBuffer(), 0, 1, &scissor);

		auto clearValues = renderStage.GetClearValues();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderStage.GetRenderpass()->GetRenderpass();
		renderPassBeginInfo.framebuffer = renderStage.GetActiveFramebuffer(m_swapchain->GetActiveImageIndex());
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(m_commandBuffers[m_swapchain->GetActiveImageIndex()]->GetCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		return true;
	}

	void Renderer::EndRenderpass(RenderStage &renderStage)
	{
		auto presentQueue = m_logicalDevice->GetPresentQueue();

		vkCmdEndRenderPass(m_commandBuffers[m_swapchain->GetActiveImageIndex()]->GetCommandBuffer());

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
			else
			{
				Renderer::CheckVk(presentResult);
			}
		}

		m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}
}
