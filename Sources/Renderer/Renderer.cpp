#include "Renderer.hpp"

#include <cassert>
#include "Helpers/FileSystem.hpp"
#include "Scenes/Scenes.hpp"
#include "IRenderer.hpp"

namespace acid
{
	Renderer::Renderer() :
		m_renderManager(nullptr),
		m_rendererRegister(RendererRegister()),
		m_renderStages(std::vector<std::unique_ptr<RenderStage>>()),
		m_swapchain(nullptr),
		m_fenceSwapchainImage(VK_NULL_HANDLE),
		m_activeSwapchainImage(UINT32_MAX),
		m_pipelineCache(VK_NULL_HANDLE),
		m_semaphore(VK_NULL_HANDLE),
		m_commandPool(VK_NULL_HANDLE),
		m_commandBuffer(nullptr)
	{
		CreateFences();
		CreateCommandPool();
		CreatePipelineCache();
	}

	Renderer::~Renderer()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto graphicsQueue = Display::Get()->GetGraphicsQueue();

		Display::CheckVk(vkQueueWaitIdle(graphicsQueue));

		vkDestroyPipelineCache(logicalDevice, m_pipelineCache, nullptr);

		vkDestroyFence(logicalDevice, m_fenceSwapchainImage, nullptr);
		vkDestroySemaphore(logicalDevice, m_semaphore, nullptr);
		vkDestroyCommandPool(logicalDevice, m_commandPool, nullptr);
	}

	void Renderer::Update()
	{
		if (Display::Get()->IsIconified() || m_renderManager == nullptr)
		{
			return;
		}

		if (!m_renderManager->m_started)
		{
			m_rendererRegister.Clear();
			CreateRenderpass(m_renderManager->m_renderpassCreates);
			m_renderManager->Start();
			m_renderManager->m_started = true;
		}

		m_renderManager->Update();

		auto camera = Scenes::Get()->GetCamera();
		auto &stages = m_rendererRegister.GetStages();
		Vector4 clipPlane = Vector4(0.0f, 1.0f, 0.0f, +std::numeric_limits<float>::infinity());

		if (stages.empty())
		{
			return;
		}

		std::optional<uint32_t> renderpass = {};
		uint32_t subpass = 0;

		for (auto &[key, renderers] : stages)
		{
			if (renderpass != key.GetRenderpass())
			{
				// Ends the previous renderpass.
				if (renderpass)
				{
					EndRenderpass(*renderpass);
				}

				renderpass = key.GetRenderpass();
				subpass = 0;

				// Starts the next renderpass.
				auto startResult = StartRenderpass(*renderpass);

				if (!startResult)
				{
					return;
				}
			}

			auto renderStage = GetRenderStage(*renderpass);

			// Changes the subpass.
			if (subpass != key.GetSubpass())
			{
				uint32_t difference = key.GetSubpass() - subpass;

				if (subpass == renderStage->SubpassCount() - 1)
				{
					difference -= 1;
				}

				for (uint32_t d = 0; d < difference; d++)
				{
					Renderer::Get()->NextSubpass();
				}

				subpass = key.GetSubpass();
			}

			// Renders subpass renderers.
			for (auto &renderer : renderers)
			{
				if (!renderer->IsEnabled())
				{
					continue;
				}

				renderer->Render(*m_commandBuffer, clipPlane, *camera);
			}
		}

		// Ends the last renderpass.
		EndRenderpass(*renderpass);
	}

	void Renderer::CreateRenderpass(const std::vector<RenderpassCreate> &renderpassCreates)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		VkExtent2D displayExtent = {
			Display::Get()->GetWidth(),
			Display::Get()->GetHeight()
		};

		m_renderStages.clear();
		m_swapchain = std::make_unique<Swapchain>(displayExtent);

		for (auto &renderpassCreate : renderpassCreates)
		{
			auto renderStage = new RenderStage(static_cast<uint32_t>(m_renderStages.size()), renderpassCreate); // TODO: Clean up.
			renderStage->Rebuild(*m_swapchain);
			m_renderStages.emplace_back(renderStage);
		}

		Display::CheckVk(vkDeviceWaitIdle(logicalDevice));

#if defined(ACID_VERBOSE)
		auto debugEnd = Engine::GetTime();
		Log::Out("Renderpass created in %ims\n", (debugEnd - debugStart).AsMilliseconds());
#endif
	}

	void Renderer::CaptureScreenshot(const std::string &filename)
	{
#if defined(ACID_VERBOSE)
		auto debugStart = Engine::GetTime();
#endif

		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		auto width = Display::Get()->GetWidth();
		auto height = Display::Get()->GetHeight();

		Log::Out("Saving screenshot to: '%s'\n", filename.c_str());

		VkImage srcImage = m_swapchain->GetImages().at(m_activeSwapchainImage);
		VkImage dstImage;
		VkDeviceMemory dstImageMemory;
		bool supportsBlit = Texture::CopyImage(srcImage, dstImage, dstImageMemory, width, height, true, 0, 1);

		// Get layout of the image (including row pitch).
		VkImageSubresource imageSubresource = {};
		imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		VkSubresourceLayout subresourceLayout;
		vkGetImageSubresourceLayout(logicalDevice, dstImage, &imageSubresource, &subresourceLayout);

		// Creates the screenshot image file.
		FileSystem::Create(filename);

		// Map image memory so we can start copying from it.
		char *data;
		vkMapMemory(logicalDevice, dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void **) &data);
		data += subresourceLayout.offset;

		// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'll have to manually swizzle color components
		bool colourSwizzle = false;

		// Check if source is BGR.
		if (!supportsBlit)
		{
			std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
			colourSwizzle = std::find(formatsBGR.begin(), formatsBGR.end(), surfaceFormat.format) != formatsBGR.end();
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
		vkUnmapMemory(logicalDevice, dstImageMemory);
		vkFreeMemory(logicalDevice, dstImageMemory, nullptr);
		vkDestroyImage(logicalDevice, dstImage, nullptr);

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

	IDescriptor *Renderer::GetAttachment(const std::string &name) const
	{
		for (auto &renderStage : m_renderStages)
		{
			auto attachment = renderStage->GetAttachment(name);

			if (attachment != nullptr)
			{
				return attachment;
			}
		}

		return nullptr;
	}

	void Renderer::CreateFences()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &m_fenceSwapchainImage);
	}

	void Renderer::CreateCommandPool()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		Display::CheckVk(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &m_semaphore));

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = Display::Get()->GetGraphicsFamily();
		Display::CheckVk(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &m_commandPool));

		m_commandBuffer = std::make_unique<CommandBuffer>(false);
	}

	void Renderer::CreatePipelineCache()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		Display::CheckVk(vkCreatePipelineCache(logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
	}

	void Renderer::RecreatePass(const uint32_t &i)
	{
		auto graphicsQueue = Display::Get()->GetGraphicsQueue();
		auto renderStage = GetRenderStage(i);

		VkExtent2D displayExtent = {
			Display::Get()->GetWidth(),
			Display::Get()->GetHeight()
		};

		Display::CheckVk(vkQueueWaitIdle(graphicsQueue));

		if (renderStage->HasSwapchain() && !m_swapchain->IsSameExtent(displayExtent))
		{
#if defined(ACID_VERBOSE)
			Log::Out("Resizing swapchain: Old (%i, %i), New (%i, %i)\n", m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, displayExtent.width, displayExtent.height);
#endif
			m_swapchain = std::make_unique<Swapchain>(displayExtent);
		}

		renderStage->Rebuild(*m_swapchain);
	}

	bool Renderer::StartRenderpass(const uint32_t &i)
	{
		auto renderStage = GetRenderStage(i);

		if (renderStage->IsOutOfDate(m_swapchain->GetExtent()))
		{
			RecreatePass(i);
			return false;
		}

		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto graphicsQueue = Display::Get()->GetGraphicsQueue();

		Display::CheckVk(vkQueueWaitIdle(graphicsQueue));

		if (renderStage->HasSwapchain())
		{
			VkResult acquireResult = vkAcquireNextImageKHR(logicalDevice, *m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), VK_NULL_HANDLE, m_fenceSwapchainImage, &m_activeSwapchainImage);

			if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
			{
				RecreatePass(i);
				return false;
			}

			if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
			{
				assert(false && "Renderer failed to acquire swapchain image!");
			}

			Display::CheckVk(vkWaitForFences(logicalDevice, 1, &m_fenceSwapchainImage, VK_TRUE, std::numeric_limits<uint64_t>::max()));

			Display::CheckVk(vkResetFences(logicalDevice, 1, &m_fenceSwapchainImage));
		}

		if (!m_commandBuffer->IsRunning())
		{
			m_commandBuffer->Begin(VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);
		}

		VkRect2D renderArea = {};
		renderArea.offset = { 0, 0 };
		renderArea.extent = {
			renderStage->GetWidth(),
			renderStage->GetHeight()
		};

		auto clearValues = renderStage->GetClearValues();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderStage->GetRenderpass()->GetRenderpass();
		renderPassBeginInfo.framebuffer = renderStage->GetActiveFramebuffer(m_activeSwapchainImage);
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();
		vkCmdBeginRenderPass(m_commandBuffer->GetCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(renderStage->GetWidth());
		viewport.height = static_cast<float>(renderStage->GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_commandBuffer->GetCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = {
			renderStage->GetWidth(),
			renderStage->GetHeight()
		};
		vkCmdSetScissor(m_commandBuffer->GetCommandBuffer(), 0, 1, &scissor);

		return true;
	}

	void Renderer::EndRenderpass(const uint32_t &i)
	{
		auto renderStage = GetRenderStage(i);
		auto presentQueue = Display::Get()->GetPresentQueue();

		vkCmdEndRenderPass(m_commandBuffer->GetCommandBuffer());

		if (!renderStage->HasSwapchain())
		{
			return;
		}

		m_commandBuffer->End();
		m_commandBuffer->Submit(m_semaphore, VK_NULL_HANDLE, false);

		std::vector<VkSemaphore> waitSemaphores = {m_semaphore};

		VkResult presentResult = VK_RESULT_MAX_ENUM;

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		presentInfo.pWaitSemaphores = waitSemaphores.data();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = m_swapchain->GetSwapchain();
		presentInfo.pImageIndices = &m_activeSwapchainImage;
		presentInfo.pResults = &presentResult;
		VkResult queuePresentResult = vkQueuePresentKHR(presentQueue, &presentInfo);

		if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR)
		{
			RecreatePass(i);
			return;
		}

		Display::CheckVk(presentResult);
		Display::CheckVk(vkQueueWaitIdle(presentQueue));
	}

	void Renderer::NextSubpass()
	{
		vkCmdNextSubpass(m_commandBuffer->GetCommandBuffer(), VK_SUBPASS_CONTENTS_INLINE);
	}
}
