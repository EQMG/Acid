#include "Renderer.hpp"

#include <cassert>
#include "../Devices/Display.hpp"
#include "Pipelines/Pipeline.hpp"

namespace Flounder
{
	Renderer::Renderer() :
		IModule(),
		m_managerRender(nullptr),
		m_swapchain(nullptr),
		m_depthStencil(nullptr),
		m_renderPass(nullptr),
		m_framebuffers(nullptr),
		m_fenceSwapchainImage(VK_NULL_HANDLE),
		m_activeSwapchainImage(UINT32_MAX),
		m_pipelineCache(VK_NULL_HANDLE),
		m_semaphore(VK_NULL_HANDLE),
		m_commandPool(VK_NULL_HANDLE),
		m_commandBuffer(VK_NULL_HANDLE)
	{
		const VkExtent2D extent2d = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight())
		};
		const VkExtent3D extent3d = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight()), 1
		};
		const auto surfaceFormat = Display::Get()->GetSurfaceFormat();

		CreateFences();
		CreateCommandPool();
		CreatePipelineCache();

		m_swapchain = new Swapchain(extent2d);
		m_depthStencil = new DepthStencil(extent3d);
		m_renderPass = new RenderPass(m_depthStencil->GetFormat(), surfaceFormat.format);
		m_framebuffers = new Framebuffers(m_renderPass->GetRenderPass(), m_depthStencil->GetImageView(), *m_swapchain, extent2d);

		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());
	}

	Renderer::~Renderer()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();

		vkQueueWaitIdle(queue);

		delete m_managerRender;

		vkDestroyPipelineCache(logicalDevice, m_pipelineCache, nullptr);

		vkDestroyFence(logicalDevice, m_fenceSwapchainImage, nullptr);
		vkDestroySemaphore(logicalDevice, m_semaphore, nullptr);
		vkDestroyCommandPool(logicalDevice, m_commandPool, nullptr);

		delete m_framebuffers;
		delete m_renderPass;
		delete m_depthStencil;
		delete m_swapchain;
	}

	void Renderer::Update()
	{
		m_managerRender->Render();
	}

	VkResult Renderer::StartRenderpass(const VkCommandBuffer &commandBuffer)
	{
		if (static_cast<int>(m_swapchain->GetExtent().width) != Display::Get()->GetWidth() ||
			static_cast<int>(m_swapchain->GetExtent().height) != Display::Get()->GetHeight())
		{
			RecreateSwapchain();
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();

		Platform::ErrorVk(vkQueueWaitIdle(queue));

		const VkResult acquireResult = vkAcquireNextImageKHR(logicalDevice, *m_swapchain->GetSwapchain(), UINT64_MAX, VK_NULL_HANDLE, m_fenceSwapchainImage, &m_activeSwapchainImage);

		if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
			return VK_ERROR_OUT_OF_DATE_KHR;
		}

		assert((acquireResult == VK_SUCCESS || acquireResult == VK_SUBOPTIMAL_KHR) && "Failed to acquire swapchain image!");

		Platform::ErrorVk(vkWaitForFences(logicalDevice, 1, &m_fenceSwapchainImage, VK_TRUE, UINT64_MAX));

		Platform::ErrorVk(vkResetFences(logicalDevice, 1, &m_fenceSwapchainImage));

		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Platform::ErrorVk(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

		const auto renderPass = m_renderPass->GetRenderPass();
		const auto activeFramebuffer = GetActiveFramebuffer();

		VkRect2D renderArea = {};
		renderArea.offset.x = 0;
		renderArea.offset.y = 0;
		renderArea.extent.width = static_cast<uint32_t>(Display::Get()->GetWidth());
		renderArea.extent.height = static_cast<uint32_t>(Display::Get()->GetHeight());

		std::array<VkClearValue, 6> clearValues = {};
		clearValues[0].depthStencil = {1.0f, 0}; // Depth.
		clearValues[1].color = {{0.0f, 0.0f, 0.0f, 0.0f}}; // Swapchain.
		clearValues[2].color = {{0.0f, 0.0f, 0.0f, 0.0f}}; // Colours.
		clearValues[3].color = {{0.0f, 0.0f, 0.0f, 0.0f}}; // Normals.
		clearValues[4].color = {{0.0f, 0.0f, 0.0f, 0.0f}}; // Materials.
		clearValues[5].color = {{0.0f}}; // Shadows.

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderPass;
		renderPassBeginInfo.framebuffer = activeFramebuffer;
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassBeginInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(Display::Get()->GetWidth());
		viewport.height = static_cast<float>(Display::Get()->GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = static_cast<uint32_t>(Display::Get()->GetWidth());
		scissor.extent.height = static_cast<uint32_t>(Display::Get()->GetHeight());
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		return VK_SUCCESS;
	}

	void Renderer::NextSubpass(const VkCommandBuffer &commandBuffer)
	{
		vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::EndRenderpass(const VkCommandBuffer &commandBuffer)
	{
		const auto queue = Display::Get()->GetQueue();

		vkCmdEndRenderPass(commandBuffer);

		Platform::ErrorVk(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_semaphore;

		const VkResult queueSubmitResult = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

		assert(queueSubmitResult == VK_SUCCESS && "Failed to acquire swapchain image!");

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
			RecreateSwapchain();
			return;
		}

		assert(result == VK_SUCCESS && "Failed to present swapchain image!");

		Platform::ErrorVk(result);
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

		Platform::ErrorVk(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &m_semaphore));

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = Display::Get()->GetGraphicsFamilyIndex();
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		Platform::ErrorVk(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &m_commandPool));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = m_commandPool;
		commandBufferAllocateInfo.commandBufferCount = 1;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		Platform::ErrorVk(vkAllocateCommandBuffers(logicalDevice, &commandBufferAllocateInfo, &m_commandBuffer));
	}

	void Renderer::CreatePipelineCache()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		Platform::ErrorVk(vkCreatePipelineCache(logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
	}

	void Renderer::RecreateSwapchain()
	{
		const auto queue = Display::Get()->GetQueue();

		const VkExtent2D extent2d = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight())
		};
		const VkExtent3D extent3d = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight()), 1
		};

#if FLOUNDER_VERBOSE
		printf("Resizing swapchain: Old (%i, %i), New (%i, %i)\n", m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, extent2d.width, extent2d.height);
#endif
		Platform::ErrorVk(vkQueueWaitIdle(queue));

		delete m_framebuffers;
		delete m_depthStencil;
		delete m_swapchain;

		m_swapchain = new Swapchain(extent2d);
		m_depthStencil = new DepthStencil(extent3d);
		m_framebuffers = new Framebuffers(m_renderPass->GetRenderPass(), m_depthStencil->GetImageView(), *m_swapchain, extent2d);
	}

	VkCommandBuffer Renderer::BeginSingleTimeCommands()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto commandPool = Renderer::Get()->GetCommandPool();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		Platform::ErrorVk(vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer));

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Platform::ErrorVk(vkBeginCommandBuffer(commandBuffer, &beginInfo));

		return commandBuffer;
	}

	void Renderer::EndSingleTimeCommands(const VkCommandBuffer &commandBuffer)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();
		const auto commandPool = Renderer::Get()->GetCommandPool();

		Platform::ErrorVk(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		Platform::ErrorVk(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

		Platform::ErrorVk(vkQueueWaitIdle(queue));

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
	}
}
