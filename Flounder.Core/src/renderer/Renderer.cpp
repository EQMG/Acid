#include "Renderer.hpp"

namespace Flounder
{
	Renderer::Renderer() :
		IModule()
	{
		m_managerRender = nullptr;

		m_renderPass = new RenderPass();
		m_swapchain = new Swapchain();

		m_commandPool = new CommandPool();
		m_commandPoolTransfer = new CommandPool();

		m_commandBuffers = std::vector<VkCommandBuffer>();
		m_commandBufferFences = std::vector<VkFence>();

		m_imageAvailableSemaphore = VK_NULL_HANDLE;
		m_renderFinishedSemaphore = VK_NULL_HANDLE;

		VkDevice logicalDevice = Display::Get()->GetVkDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetVkPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetVkSurface();
		GLFWwindow *window = Display::Get()->GetGlfwWindow();

		m_swapchain->Create(&logicalDevice, &physicalDevice, &surface, window);
		m_renderPass->Create(&logicalDevice, m_swapchain->GetImageFormat());
		m_swapchain->CreateFramebuffers(&logicalDevice, m_renderPass->GetRenderPass());
		QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(&physicalDevice, &surface);
		m_commandPool->Create(&logicalDevice, &physicalDevice, &surface, indices.graphicsFamily);
		m_commandPoolTransfer->Create(&logicalDevice, &physicalDevice, &surface, indices.transferFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
		CreateCommandBuffers();
		CreateSemaphores();
	}

	Renderer::~Renderer()
	{
		VkDevice logicalDevice = Display::Get()->GetVkDevice();

		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(logicalDevice);

		delete m_managerRender;
		m_swapchain->CleanupFramebuffers(&logicalDevice);
		m_swapchain->Cleanup(&logicalDevice);
		delete m_swapchain;

		vkFreeCommandBuffers(logicalDevice, *m_commandPool->GetCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
		
		m_renderPass->Cleanup(&logicalDevice);
		delete m_renderPass;

		m_commandPool->Cleanup(&logicalDevice);
		delete m_commandPool;
		m_commandPoolTransfer->Cleanup(&logicalDevice);
		delete m_commandPoolTransfer;

		vkDestroySemaphore(logicalDevice, m_renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(logicalDevice, m_imageAvailableSemaphore, nullptr);

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
		//	vkDestroyBuffer(logicalDevice, m_commandBuffers[i], nullptr);
			vkDestroyFence(logicalDevice, m_commandBufferFences[i], nullptr);
		}
	}

	void Renderer::CreateCommandBuffers()
	{
		m_commandBuffers.resize(m_swapchain->GetFramebufferSize());
		m_commandBufferFences.resize(m_swapchain->GetFramebufferSize());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *m_commandPool->GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

		GlfwVulkan::ErrorCheck(vkAllocateCommandBuffers(Display::Get()->GetVkDevice(), &allocInfo, m_commandBuffers.data()));

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		GlfwVulkan::ErrorCheck(vkCreateFence(Display::Get()->GetVkDevice(), &fenceCreateInfo, nullptr, m_commandBufferFences.data()));

		std::cout << "Command buffers recorded successfully!" << std::endl;
	}

	void Renderer::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		GlfwVulkan::ErrorCheck(vkCreateSemaphore(Display::Get()->GetVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));

		GlfwVulkan::ErrorCheck(vkCreateSemaphore(Display::Get()->GetVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));

		std::cout << "Semaphores recorded successfully!" << std::endl;
	}

	void Renderer::Update()
	{
		if (m_managerRender != nullptr)
		{
	//		m_managerRender->Render();
		}
	}

	void Renderer::BeginReindering()
	{
		uint32_t imageIndex;
		GlfwVulkan::ErrorCheck(vkAcquireNextImageKHR(Display::Get()->GetVkDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

		VkDevice logicalDevice = Display::Get()->GetVkDevice();

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			GlfwVulkan::ErrorCheck(vkWaitForFences(logicalDevice, 1, &m_commandBufferFences[i], VK_TRUE, UINT64_MAX));
			GlfwVulkan::ErrorCheck(vkResetFences(logicalDevice, 1, &m_commandBufferFences[i]));

			GlfwVulkan::ErrorCheck(vkAcquireNextImageKHR(logicalDevice, m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

			int width = Display::Get()->GetWidth();
			int height = Display::Get()->GetHeight();

			VkRect2D renderArea;
			renderArea.offset.x = 0;
			renderArea.offset.y = 0;
			renderArea.extent.width = width;
			renderArea.extent.height = height;

			VkClearValue clearColour = { 0.0f, 0.0f, 0.0f, 1.0f };

			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.renderArea = renderArea;
			renderPassBeginInfo.renderPass = m_renderPass->GetRenderPass();
			renderPassBeginInfo.framebuffer = m_swapchain->GetFramebuffer(static_cast<uint32_t>(i));
			renderPassBeginInfo.clearValueCount = 1;
			renderPassBeginInfo.pClearValues = &clearColour;

			vkCmdSetScissor(m_commandBuffers[i], 0, 1, &renderArea);

			VkViewport viewport;
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(width);
			viewport.height = static_cast<float>(height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(m_commandBuffers[i], 0, 1, &viewport);

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		}
	}

	void Renderer::NextSubpass()
	{
		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			vkCmdNextSubpass(m_commandBuffers[i], VK_SUBPASS_CONTENTS_INLINE);
		}
	}

	void Renderer::EndRendering()
	{
		uint32_t imageIndex;
		GlfwVulkan::ErrorCheck(vkAcquireNextImageKHR(Display::Get()->GetVkDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			vkCmdEndRenderPass(m_commandBuffers[i]);
			GlfwVulkan::ErrorCheck(vkEndCommandBuffer(m_commandBuffers[i]));

			VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
			VkSubmitInfo submit_info = {};
			submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers = &m_commandBuffers[i];
			submit_info.waitSemaphoreCount = 1;
			submit_info.pWaitSemaphores = waitSemaphores;
			submit_info.pWaitDstStageMask = &waitDstStageMask;

			GlfwVulkan::ErrorCheck(vkQueueSubmit(Display::Get()->GetVkDisplayQueue(), 1, &submit_info, m_commandBufferFences[i]));

		//	commandBufferSubmitted[currentCommandBuffer] = true;
		//	currentCommandBuffer = (currentCommandBuffer + 1) % NUM_COMMAND_BUFFERS;

			VkSwapchainKHR swapChains[] = { m_swapchain->GetSwapchain() };
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			vkQueueWaitIdle(Display::Get()->GetVkDisplayQueue());
			GlfwVulkan::ErrorCheck(vkQueuePresentKHR(Display::Get()->GetVkDisplayQueue(), &presentInfo));
		}
	}
}
