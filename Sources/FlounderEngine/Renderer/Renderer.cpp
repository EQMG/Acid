#include "Renderer.hpp"

#include <cassert>
#include "../Devices/Display.hpp"
#include "Pipelines/Pipeline.hpp"

namespace Flounder
{
	Renderer::Renderer() :
		IModule(),
		m_managerRender(nullptr),
		m_passes(std::vector<Pass *>()),
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

		vkDestroyPipelineCache(logicalDevice, m_pipelineCache, nullptr);

		vkDestroyFence(logicalDevice, m_fenceSwapchainImage, nullptr);
		vkDestroySemaphore(logicalDevice, m_semaphore, nullptr);
		vkDestroyCommandPool(logicalDevice, m_commandPool, nullptr);

		for (auto pass : m_passes)
		{
			delete pass;
		}

		delete m_swapchain;
	}

	void Renderer::Update()
	{
		m_managerRender->Render();
	}

	void Renderer::CreateRenderpass(std::vector<RenderpassCreate *> renderpassCreates)
	{
		const auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		const VkExtent2D extent2d = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight())
		};
		const VkExtent3D extent3d = {
			static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight()), 1
		};

		m_passes.clear();
		m_swapchain = new Swapchain(*renderpassCreates.back(), extent2d);

		for (auto &renderpassCreate : renderpassCreates)
		{
			auto pass = new Pass(renderpassCreate);
			pass->m_depthStencil = new DepthStencil(extent3d);
			pass->m_renderpass = new Renderpass(*renderpassCreate, pass->m_depthStencil->GetFormat(), surfaceFormat.format);
			pass->m_framebuffers = new Framebuffers(pass->m_renderpass->GetRenderpass(), pass->m_depthStencil->GetImageView(), *m_swapchain, extent2d);
			m_passes.push_back(pass);
		}

		vkDeviceWaitIdle(Display::Get()->GetLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetQueue());
	}

	VkResult Renderer::StartRenderpass(const VkCommandBuffer &commandBuffer, const unsigned int &i)
	{
		const auto pass = GetPass(i);

		if (pass->IsOutOfDate(m_swapchain->GetExtent()))
		{
			RecreatePass(i);
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto queue = Display::Get()->GetQueue();

		Platform::ErrorVk(vkQueueWaitIdle(queue));

		const VkResult acquireResult = vkAcquireNextImageKHR(logicalDevice, *m_swapchain->GetSwapchain(), UINT64_MAX, VK_NULL_HANDLE, m_fenceSwapchainImage, &m_activeSwapchainImage);

		if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreatePass(i);
			return VK_ERROR_OUT_OF_DATE_KHR;
		}

		assert((acquireResult == VK_SUCCESS || acquireResult == VK_SUBOPTIMAL_KHR) && "Failed to acquire swapchain image!");

		Platform::ErrorVk(vkWaitForFences(logicalDevice, 1, &m_fenceSwapchainImage, VK_TRUE, UINT64_MAX));

		Platform::ErrorVk(vkResetFences(logicalDevice, 1, &m_fenceSwapchainImage));

		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		Platform::ErrorVk(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

		VkRect2D renderArea = {};
		renderArea.offset.x = 0;
		renderArea.offset.y = 0;
		renderArea.extent.width = pass->GetWidth();
		renderArea.extent.height = pass->GetHeight();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = pass->m_renderpass->GetRenderpass();
		renderPassBeginInfo.framebuffer = pass->GetActiveFramebuffer(m_activeSwapchainImage);
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(pass->m_clearValues.size());
		renderPassBeginInfo.pClearValues = pass->m_clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(pass->GetWidth());
		viewport.height = static_cast<float>(pass->GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = pass->GetWidth();
		scissor.extent.height = pass->GetHeight();
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		return VK_SUCCESS;
	}

	void Renderer::EndRenderpass(const VkCommandBuffer &commandBuffer, const unsigned int &i)
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
			RecreatePass(i);
			return;
		}

		assert(result == VK_SUCCESS && "Failed to present swapchain image!");

		Platform::ErrorVk(result);
	}

	void Renderer::NextSubpass(const VkCommandBuffer &commandBuffer)
	{
		vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
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

	void Renderer::RecreatePass(const int &i)
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

		if (i == 1)
		{
			delete m_swapchain;
			m_swapchain = new Swapchain(*m_passes.back()->m_renderpassCreate, extent2d);
		}

		GetPass(i)->Rebuild(m_swapchain, extent2d, extent3d);
	}
}
