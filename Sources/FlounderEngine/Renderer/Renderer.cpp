#include "Renderer.hpp"

namespace fl
{
	Renderer::Renderer() :
		IModule(),
		m_managerRender(nullptr),
		m_renderStages(std::vector<RenderStage *>()),
		m_swapchain(nullptr),
		m_fenceSwapchainImage(VK_NULL_HANDLE),
		m_activeSwapchainImage(UINT32_MAX),
		m_pipelineCache(VK_NULL_HANDLE),
		m_semaphore(VK_NULL_HANDLE),
		m_commandPool(VK_NULL_HANDLE),
		m_commandBuffer(VK_NULL_HANDLE)
	{
		CreateFences();
		CreateCommandPool();
		CreatePipelineCache();
	}

	Renderer::~Renderer()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();

		vkQueueWaitIdle(queue);

		delete m_managerRender;

		for (auto renderStage : m_renderStages)
		{
			delete renderStage;
		}

		delete m_swapchain;

		vkDestroyPipelineCache(logicalDevice, m_pipelineCache, nullptr);

		vkDestroyFence(logicalDevice, m_fenceSwapchainImage, nullptr);
		vkDestroySemaphore(logicalDevice, m_semaphore, nullptr);
		vkDestroyCommandPool(logicalDevice, m_commandPool, nullptr);
	}

	void Renderer::Update()
	{
		if (Display::Get()->IsIconified())
		{
			return;
		}

		m_managerRender->Render();
	}

	void Renderer::CreateRenderpass(std::vector<RenderpassCreate *> renderpassCreates)
	{
#if FL_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		const VkExtent2D displayExtent2D = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight())
		};

		m_renderStages.clear();
		m_swapchain = new Swapchain(displayExtent2D);

		for (auto &renderpassCreate : renderpassCreates)
		{
			auto renderStage = new RenderStage(m_renderStages.size(), renderpassCreate);
			renderStage->Rebuild(m_swapchain);
			m_renderStages.push_back(renderStage);
		}

		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());

#if FL_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Renderpass created in %fms\n", debugEnd - debugStart);
#endif
	}

	VkResult Renderer::StartRenderpass(const VkCommandBuffer &commandBuffer, const unsigned int &i)
	{
		const auto renderStage = GetRenderStage(i);

		if (renderStage->IsOutOfDate(m_swapchain->GetExtent()))
		{
			RecreatePass(i);
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();

		Display::ErrorVk(vkQueueWaitIdle(queue));

		if (renderStage->m_hasSwapchain)
		{
			const VkResult acquireResult = vkAcquireNextImageKHR(logicalDevice, *m_swapchain->GetSwapchain(), UINT64_MAX, VK_NULL_HANDLE, m_fenceSwapchainImage, &m_activeSwapchainImage);

			if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
			{
				RecreatePass(i);
				return VK_ERROR_OUT_OF_DATE_KHR;
			}

			if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
			{
				throw std::runtime_error("Renderer failed to acquire swapchain image!");
			}

			Display::ErrorVk(vkWaitForFences(logicalDevice, 1, &m_fenceSwapchainImage, VK_TRUE, UINT64_MAX));

			Display::ErrorVk(vkResetFences(logicalDevice, 1, &m_fenceSwapchainImage));
		}

		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Display::ErrorVk(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

		VkRect2D renderArea = {};
		renderArea.offset.x = 0;
		renderArea.offset.y = 0;
		renderArea.extent.width = renderStage->GetWidth();
		renderArea.extent.height = renderStage->GetHeight();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderStage->m_renderpass->GetRenderpass();
		renderPassBeginInfo.framebuffer = renderStage->GetActiveFramebuffer(m_activeSwapchainImage);
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(renderStage->m_clearValues.size());
		renderPassBeginInfo.pClearValues = renderStage->m_clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(renderStage->GetWidth());
		viewport.height = static_cast<float>(renderStage->GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = renderStage->GetWidth();
		scissor.extent.height = renderStage->GetHeight();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		return VK_SUCCESS;
	}

	void Renderer::EndRenderpass(const VkCommandBuffer &commandBuffer, const unsigned int &i)
	{
		const auto renderStage = GetRenderStage(i);
		const auto queue = Display::Get()->GetQueue();

		vkCmdEndRenderPass(commandBuffer);
		Display::ErrorVk(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		if (renderStage->m_hasSwapchain)
		{
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &m_semaphore;
		}

		const VkResult queueSubmitResult = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

		if (queueSubmitResult != VK_SUCCESS)
		{
			throw std::runtime_error("Renderer failed to acquire swapchain image!");
		}

		Display::ErrorVk(vkQueueWaitIdle(queue));

		if (!renderStage->m_hasSwapchain)
		{
			return;
		}

		std::vector<VkSemaphore> waitSemaphores = {m_semaphore};

		VkResult result = VK_RESULT_MAX_ENUM;

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		presentInfo.pWaitSemaphores = waitSemaphores.data();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = m_swapchain->GetSwapchain();
		presentInfo.pImageIndices = &m_activeSwapchainImage;
		presentInfo.pResults = &result;

		const VkResult queuePresentResult = vkQueuePresentKHR(queue, &presentInfo);

		if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR)
		{
			RecreatePass(i);
			return;
		}

#ifndef FL_BUILD_MACOS
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Renderer failed to present swapchain image!");
		}
#endif

		Display::ErrorVk(result);
		vkQueueWaitIdle(queue);
	}

	void Renderer::NextSubpass(const VkCommandBuffer &commandBuffer)
	{
		vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	VkCommandBuffer Renderer::BeginSingleTimeCommands(const VkCommandBufferLevel &level)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto commandPool = Renderer::Get()->GetCommandPool();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = level;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		Display::ErrorVk(vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer));

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Display::ErrorVk(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		return commandBuffer;
	}

	void Renderer::EndSingleTimeCommands(const VkCommandBuffer &commandBuffer)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();
		const auto commandPool = Renderer::Get()->GetCommandPool();

		Display::ErrorVk(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		Display::ErrorVk(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

		Display::ErrorVk(vkQueueWaitIdle(queue));

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	}

	uint32_t Renderer::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *deviceMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags &requiredProperties)
	{
		for (uint32_t i = 0; i < deviceMemoryProperties->memoryTypeCount; ++i)
		{
			if (memoryRequirements->memoryTypeBits & (1 << i))
			{
				if ((deviceMemoryProperties->memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties)
				{
					return i;
				}
			}
		}

//#ifdef FL_BUILD_WINDOWS
//		MessageBox(nullptr, "Couldn't find proper memory type!", "Vulkan Error", 0);
//#endif
		throw std::runtime_error("Vulkan runtime error, couldn't find proper memory type!");
		return UINT32_MAX;
	}

	void Renderer::CreateFences()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &m_fenceSwapchainImage);
	}

	void Renderer::CreateCommandPool()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		Display::ErrorVk(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &m_semaphore));

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = Display::Get()->GetGraphicsFamilyIndex();
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		Display::ErrorVk(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &m_commandPool));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = m_commandPool;
		commandBufferAllocateInfo.commandBufferCount = 1;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		Display::ErrorVk(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &m_commandBuffer));
	}

	void Renderer::CreatePipelineCache()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		Display::ErrorVk(vkCreatePipelineCache(logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
	}

	void Renderer::RecreatePass(const int &i)
	{
		const auto queue = Display::Get()->GetQueue();
		const auto renderStage = GetRenderStage(i);

		const VkExtent2D displayExtent2D = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight())
		};

		Display::ErrorVk(vkQueueWaitIdle(queue));

		if (renderStage->m_hasSwapchain && !m_swapchain->SameExtent(displayExtent2D))
		{
#if FL_VERBOSE
			printf("Resizing swapchain: Old (%i, %i), New (%i, %i)\n", m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, displayExtent2D.width, displayExtent2D.height);
#endif
			delete m_swapchain;
			m_swapchain = new Swapchain(displayExtent2D);
		}

		renderStage->Rebuild(m_swapchain);
	}
}
