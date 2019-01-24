#include "Renderer.hpp"

#include <cassert>
#include <SPIRV/GlslangToSpv.h>
#include "Renderer/Renderer.hpp"
#include "Helpers/FileSystem.hpp"
#include "RenderPipeline.hpp"

namespace acid
{
	Renderer::Renderer() :
		m_renderManager(nullptr),
		m_renderStages(std::vector<std::unique_ptr<RenderStage>>()),
		m_swapchain(nullptr),
		m_pipelineCache(VK_NULL_HANDLE),
		m_commandPool(VK_NULL_HANDLE),
		m_presentSemaphore(VK_NULL_HANDLE),
		m_commandBuffer(nullptr),
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
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto graphicsQueue = logicalDevice->GetGraphicsQueue();

		Renderer::CheckVk(vkQueueWaitIdle(graphicsQueue));

		glslang::FinalizeProcess();

		vkDestroyPipelineCache(logicalDevice->GetLogicalDevice(), m_pipelineCache, nullptr);

		vkDestroyCommandPool(logicalDevice->GetLogicalDevice(), m_commandPool, nullptr);
		vkDestroySemaphore(logicalDevice->GetLogicalDevice(), m_presentSemaphore, nullptr);
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
				auto startResult = StartRenderpass(*GetRenderStage(*renderpass));

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
					vkCmdNextSubpass(m_commandBuffer->GetCommandBuffer(), VK_SUBPASS_CONTENTS_INLINE);
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

				renderPipeline->Render(*m_commandBuffer);
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

		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto surface = Renderer::Get()->GetSurface();
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
		vkGetImageSubresourceLayout(logicalDevice->GetLogicalDevice(), dstImage, &imageSubresource, &subresourceLayout);

		// Creates the screenshot image file.
		FileSystem::Create(filename);

		// Map image memory so we can start copying from it.
		char *data;
		vkMapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void **) &data);
		data += subresourceLayout.offset;

		// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'll have to manually swizzle color components
		bool colourSwizzle = false;

		// Check if source is BGR.
		if (!supportsBlit)
		{
			std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
			colourSwizzle = std::find(formatsBGR.begin(), formatsBGR.end(), surface->GetFormat().format) != formatsBGR.end();
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
		vkUnmapMemory(logicalDevice->GetLogicalDevice(), dstImageMemory);
		vkFreeMemory(logicalDevice->GetLogicalDevice(), dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice->GetLogicalDevice(), dstImage, nullptr);

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
		for (const auto &renderStage : m_renderStages)
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
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto graphicsFamily = logicalDevice->GetGraphicsFamily();

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		Renderer::CheckVk(vkCreateSemaphore(logicalDevice->GetLogicalDevice(), &semaphoreCreateInfo, nullptr, &m_presentSemaphore));

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = graphicsFamily;
		Renderer::CheckVk(vkCreateCommandPool(logicalDevice->GetLogicalDevice(), &commandPoolCreateInfo, nullptr, &m_commandPool));

		m_commandBuffer = std::make_unique<CommandBuffer>(false);
	}

	void Renderer::CreatePipelineCache()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		Renderer::CheckVk(vkCreatePipelineCache(logicalDevice->GetLogicalDevice(), &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
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
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto graphicsQueue = logicalDevice->GetGraphicsQueue();

		VkExtent2D displayExtent = {
			Window::Get()->GetWidth(),
			Window::Get()->GetHeight()
		};

		Renderer::CheckVk(vkQueueWaitIdle(graphicsQueue));

		if (renderStage.HasSwapchain() && !m_swapchain->IsSameExtent(displayExtent))
		{
#if defined(ACID_VERBOSE)
			Log::Out("Resizing swapchain: Old (%i, %i), New (%i, %i)\n", m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, displayExtent.width, displayExtent.height);
#endif
			m_swapchain = std::make_unique<Swapchain>(displayExtent);
		}

		renderStage.Rebuild(*m_swapchain);
	}

	bool Renderer::StartRenderpass(RenderStage &renderStage)
	{
		if (renderStage.IsOutOfDate(m_swapchain->GetExtent()))
		{
			RecreatePass(renderStage);
			return false;
		}

		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto graphicsQueue = logicalDevice->GetGraphicsQueue();

		Renderer::CheckVk(vkQueueWaitIdle(graphicsQueue));

		if (renderStage.HasSwapchain())
		{
			VkResult acquireResult = m_swapchain->AcquireNextImage();

			if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
			{
				RecreatePass(renderStage);
				return false;
			}

			if (acquireResult != VK_SUCCESS)
			{
				return false;
			}
		}

		if (!m_commandBuffer->IsRunning())
		{
			m_commandBuffer->Begin(VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
		}

		VkRect2D renderArea = {};
		renderArea.offset = { 0, 0 };
		renderArea.extent = {
			renderStage.GetWidth(),
			renderStage.GetHeight()
		};

		auto clearValues = renderStage.GetClearValues();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderStage.GetRenderpass()->GetRenderpass();
		renderPassBeginInfo.framebuffer = renderStage.GetActiveFramebuffer(m_swapchain->GetActiveImageIndex());
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(m_commandBuffer->GetCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(renderArea.extent.width);
		viewport.height = static_cast<float>(renderArea.extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_commandBuffer->GetCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = renderArea.extent;
		vkCmdSetScissor(m_commandBuffer->GetCommandBuffer(), 0, 1, &scissor);

		return true;
	}

	void Renderer::EndRenderpass(RenderStage &renderStage)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();
		auto presentQueue = logicalDevice->GetPresentQueue();

		vkCmdEndRenderPass(m_commandBuffer->GetCommandBuffer());

		if (!renderStage.HasSwapchain())
		{
			return;
		}

		m_commandBuffer->End();
		m_commandBuffer->Submit(m_presentSemaphore, VK_NULL_HANDLE, false);

		VkSemaphore waitSemaphores[1] = { m_presentSemaphore };
		VkSwapchainKHR swapchains[1] = { m_swapchain->GetSwapchain() };
		uint32_t imageIndices[1] = { m_swapchain->GetActiveImageIndex() };

		VkResult presentResult = VK_RESULT_MAX_ENUM;

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = waitSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = imageIndices;
		presentInfo.pResults = &presentResult;
		VkResult queuePresentResult = vkQueuePresentKHR(presentQueue, &presentInfo);

		if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR)
		{
			RecreatePass(renderStage);
			return;
		}

		Renderer::CheckVk(presentResult);
		Renderer::CheckVk(vkQueueWaitIdle(presentQueue));
	}
}
