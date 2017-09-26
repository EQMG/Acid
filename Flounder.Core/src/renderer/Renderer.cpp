#include "Renderer.hpp"

namespace Flounder
{
	Renderer::Renderer() :
		IModule(),
		m_managerRender(nullptr),

		m_swapchain(Swapchain()),
		m_depthStencil(DepthStencil()),
		m_renderPass(RenderPass()),
		m_fenceSwapchainImage(VK_NULL_HANDLE),
		m_activeSwapchinImage(UINT32_MAX),

		m_semaphore(VK_NULL_HANDLE),
		m_commandPool(VK_NULL_HANDLE),
		m_commandBuffer(VK_NULL_HANDLE)
	{
		const auto device = Display::Get()->GetDevice();
		const auto physicalDevice = Display::Get()->GetPhysicalDevice();
		const auto surface = Display::Get()->GetSurface();

		VkExtent2D extent2d = { static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight()) };
		VkExtent3D extent3d = { static_cast<uint32_t>(Display::Get()->GetWidth()), static_cast<uint32_t>(Display::Get()->GetHeight()), 1 };

		m_swapchain.Create(device, physicalDevice, surface, Display::Get()->GetSurfaceCapabilities(), Display::Get()->GetSurfaceFormat(), extent2d);
		m_depthStencil.Create(device, physicalDevice, Display::Get()->GetPhysicalDeviceMemoryProperties(), extent3d);
		m_renderPass.Create(device, m_depthStencil.GetFormat(), Display::Get()->GetSurfaceFormat().format);
		m_swapchain.CreateFrameBuffers(device, m_renderPass.GetRenderPass(), m_depthStencil.GetImageView(), extent2d);

		CreateFences();
		CreateCommandPool();
	}

	Renderer::~Renderer()
	{
		const auto device = Display::Get()->GetDevice();
		const auto queue = Display::Get()->GetQueue();

		vkQueueWaitIdle(queue);

		delete m_managerRender;

		vkDestroyFence(device, m_fenceSwapchainImage, nullptr);
		vkDestroySemaphore(device, m_semaphore, nullptr);
		vkDestroyCommandPool(device, m_commandPool, nullptr);

		m_swapchain.CleanupFrameBuffers(device);
		m_renderPass.Cleanup(device);
		m_depthStencil.Cleanup(device);
		m_swapchain.Cleanup(device);
	}

	void Renderer::Update()
	{
		BeginReindering();

		const auto queue = Display::Get()->GetQueue();
		const auto renderPass = m_renderPass.GetRenderPass();
		const auto activeFramebuffer = GetActiveFramebuffer();

		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfo);
		{
			VkRect2D renderArea = {};
			renderArea.offset.x = 0;
			renderArea.offset.y = 0;
			renderArea.extent.width = Display::Get()->GetWidth();
			renderArea.extent.height = Display::Get()->GetHeight();

			std::array<VkClearValue, 2> clearValues = {};
			clearValues[0].depthStencil.depth = 1.0f;
			clearValues[0].depthStencil.stencil = 0;
			clearValues[1].color.float32[0] = 0.1f; // R.
			clearValues[1].color.float32[1] = 0.1f; // G.
			clearValues[1].color.float32[2] = 0.1f; // B.
			clearValues[1].color.float32[3] = 1.0f; // A.

			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.renderPass = renderPass;
			renderPassBeginInfo.framebuffer = activeFramebuffer;
			renderPassBeginInfo.renderArea = renderArea;
			renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassBeginInfo.pClearValues = clearValues.data();
			vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			if (m_managerRender != nullptr)
			{
				VkViewport viewport = {};
				viewport.maxDepth = 1.0f;
				viewport.minDepth = 0.0f;
				viewport.width = static_cast<float>(Display::Get()->GetWidth());
				viewport.height = static_cast<float>(Display::Get()->GetHeight());
				viewport.x = 0.0f;
				viewport.y = 0.0f;
				vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

				VkRect2D scissor = {};
				scissor.extent.width = Display::Get()->GetWidth();
				scissor.extent.height = Display::Get()->GetHeight();
				scissor.offset.x = 0;
				scissor.offset.y = 0;
				vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

				m_managerRender->Render(&m_commandBuffer);
			}
			vkCmdEndRenderPass(m_commandBuffer);
		}
		vkEndCommandBuffer(m_commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_semaphore;
		vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

		EndRendering({ m_semaphore });
	}

	void Renderer::BeginReindering()
	{
		const auto device = Display::Get()->GetDevice();
		const auto queue = Display::Get()->GetQueue();

		GlfwVulkan::ErrorVk(vkQueueWaitIdle(queue));
		GlfwVulkan::ErrorVk(vkAcquireNextImageKHR(device, *m_swapchain.GetSwapchain(), UINT64_MAX, VK_NULL_HANDLE, m_fenceSwapchainImage, &m_activeSwapchinImage));
		GlfwVulkan::ErrorVk(vkWaitForFences(device, 1, &m_fenceSwapchainImage, VK_TRUE, UINT64_MAX));
		GlfwVulkan::ErrorVk(vkResetFences(device, 1, &m_fenceSwapchainImage));
	}

	void Renderer::NextSubpass()
	{
		vkCmdNextSubpass(m_commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::EndRendering(std::vector<VkSemaphore> waitSemaphores)
	{
		const auto queue = Display::Get()->GetQueue();

		VkResult result = VK_RESULT_MAX_ENUM;

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
		presentInfo.pWaitSemaphores = waitSemaphores.data();
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = m_swapchain.GetSwapchain();
		presentInfo.pImageIndices = &m_activeSwapchinImage;
		presentInfo.pResults = &result;

		GlfwVulkan::ErrorVk(vkQueuePresentKHR(queue, &presentInfo));

		GlfwVulkan::ErrorVk(result);
	}

	void Renderer::CreateFences()
	{
		const auto device = Display::Get()->GetDevice();

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vkCreateFence(device, &fenceCreateInfo, nullptr, &m_fenceSwapchainImage);
	}

	void Renderer::CreateCommandPool()
	{
		const auto device = Display::Get()->GetDevice();

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_semaphore);

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = Display::Get()->GetGraphicsFamilyIndex();
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		GlfwVulkan::ErrorVk(vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &m_commandPool));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = m_commandPool;
		commandBufferAllocateInfo.commandBufferCount = 1;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &m_commandBuffer);
	}
}
