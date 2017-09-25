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

		VkDevice logicalDevice = Display::Get()->GetDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetSurface();
		GLFWwindow *window = Display::Get()->GetWindow();

		m_swapchain->Create(&logicalDevice, &physicalDevice, &surface, window);
		m_renderPass->Create(&logicalDevice, m_swapchain->GetImageFormat());
		m_swapchain->CreateFramebuffers(&logicalDevice, m_renderPass->GetRenderPass());
		QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(&physicalDevice, &surface);
		m_commandPool->Create(&logicalDevice, &physicalDevice, &surface, indices.graphicsFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		m_commandPoolTransfer->Create(&logicalDevice, &physicalDevice, &surface, indices.transferFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
		CreateCommandBuffers();
		CreateSemaphores();
	}

	Renderer::~Renderer()
	{
		VkDevice logicalDevice = Display::Get()->GetDevice();

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

		GlfwVulkan::ErrorVk(vkAllocateCommandBuffers(Display::Get()->GetDevice(), &allocInfo, m_commandBuffers.data()));

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		GlfwVulkan::ErrorVk(vkCreateFence(Display::Get()->GetDevice(), &fenceCreateInfo, nullptr, m_commandBufferFences.data()));

		std::cout << "Command buffers recorded successfully!" << std::endl;
	}

	void Renderer::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		GlfwVulkan::ErrorVk(vkCreateSemaphore(Display::Get()->GetDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));

		GlfwVulkan::ErrorVk(vkCreateSemaphore(Display::Get()->GetDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));

		std::cout << "Semaphores recorded successfully!" << std::endl;
	}

	void Renderer::Update()
	{
		if (m_managerRender != nullptr)
		{
			m_managerRender->Render(&m_commandBuffers[0]);
		}
	}

	void Renderer::BeginReindering()
	{
		/*VkDevice logicalDevice = Display::Get()->GetVkDevice();
		int displayWidth = Display::Get()->GetWidth();
		int displayHeight = Display::Get()->GetHeight();

		for (size_t i = 0; i < m_commandBufferFences.size(); i++)
		{
			GlfwVulkan::ErrorVk(vkWaitForFences(logicalDevice, 1, &m_commandBufferFences[i], VK_TRUE, UINT64_MAX));
			GlfwVulkan::ErrorVk(vkResetFences(logicalDevice, 1, &m_commandBufferFences[i]));
		}

		uint32_t imageIndex;
		GlfwVulkan::ErrorVk(vkAcquireNextImageKHR(Display::Get()->GetVkDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

			GlfwVulkan::ErrorVk(vkAcquireNextImageKHR(logicalDevice, m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

			VkRect2D renderArea;
			renderArea.offset.x = 0;
			renderArea.offset.y = 0;
			renderArea.extent.width = displayWidth;
			renderArea.extent.height = displayHeight;

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
			viewport.width = static_cast<float>(displayWidth);
			viewport.height = static_cast<float>(displayHeight);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(m_commandBuffers[i], 0, 1, &viewport);

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		}*/
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(Display::Get()->GetDevice(), m_swapchain->GetSwapchin(), UINT32_MAX, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		VkDevice logicalDevice = Display::Get()->GetDevice();
		VkCommandBuffer command_buffer = m_commandBuffers[0];

		GlfwVulkan::ErrorVk(vkAcquireNextImageKHR(
			logicalDevice,
			m_swapchain->GetSwapchain(),
			UINT64_MAX,
			VK_NULL_HANDLE,
			m_commandBufferFences[0],
			&imageIndex));
		GlfwVulkan::ErrorVk(vkWaitForFences(logicalDevice, 1, &m_commandBufferFences[0], VK_TRUE, UINT64_MAX));
		GlfwVulkan::ErrorVk(vkResetFences(logicalDevice, 1, &m_commandBufferFences[0]));
		GlfwVulkan::ErrorVk(vkQueueWaitIdle(Display::Get()->GetDisplayQueue()));

		// Record command buffer
		VkCommandBufferBeginInfo command_buffer_begin_info{};
		command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

		VkRect2D render_area{};
		render_area.offset.x = 0;
		render_area.offset.y = 0;
		render_area.extent = m_swapchain->GetExtent();

		std::array<VkClearValue, 2> clear_values{};
		clear_values[0].depthStencil.depth = 0.0f;
		clear_values[0].depthStencil.stencil = 0;
		clear_values[1].color.float32[0] = 1.0f;
		clear_values[1].color.float32[1] = 0.0f;
		clear_values[1].color.float32[2] = 0.0f;
		clear_values[1].color.float32[3] = 1.0f;

		VkRenderPassBeginInfo render_pass_begin_info{};
		render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_begin_info.renderPass = m_renderPass->GetRenderPass();
		render_pass_begin_info.framebuffer = m_swapchain->GetFramebuffer(0);
		render_pass_begin_info.renderArea = render_area;
		render_pass_begin_info.clearValueCount = clear_values.size();
		render_pass_begin_info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

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
		/*uint32_t imageIndex;
		GlfwVulkan::ErrorVk(vkAcquireNextImageKHR(Display::Get()->GetVkDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			vkCmdEndRenderPass(m_commandBuffers[i]);
			GlfwVulkan::ErrorVk(vkEndCommandBuffer(m_commandBuffers[i]));

			VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
			VkSubmitInfo submit_info = {};
			submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers = &m_commandBuffers[i];
			submit_info.waitSemaphoreCount = 1;
			submit_info.pWaitSemaphores = waitSemaphores;
			submit_info.pWaitDstStageMask = &waitDstStageMask;

			GlfwVulkan::ErrorVk(vkQueueSubmit(Display::Get()->GetVkDisplayQueue(), 1, &submit_info, m_commandBufferFences[i]));

		//	commandBufferSubmitted[currentCommandBuffer] = true;
		//	currentCommandBuffer = (currentCommandBuffer + 1) % NUM_COMMAND_BUFFERS;

			VkSwapchainKHR swapChains[] = { m_swapchain->GetSwapchain() };
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			vkQueueWaitIdle(Display::Get()->GetVkDisplayQueue());
			GlfwVulkan::ErrorVk(vkQueuePresentKHR(Display::Get()->GetVkDisplayQueue(), &presentInfo));
		}*/
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(Display::Get()->GetDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		VkCommandBuffer command_buffer = m_commandBuffers[0];

		vkCmdEndRenderPass(command_buffer);

		vkEndCommandBuffer(command_buffer);

		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };

		// Submit command buffer
		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = 0;
		submit_info.pWaitSemaphores = nullptr;
		submit_info.pWaitDstStageMask = nullptr;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer;
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = signalSemaphores;

		vkQueueSubmit(Display::Get()->GetDisplayQueue(), 1, &submit_info, VK_NULL_HANDLE);

		VkResult present_result = VK_RESULT_MAX_ENUM;

		VkSwapchainKHR swapChains[] = { m_swapchain->GetSwapchain() };

		VkPresentInfoKHR present_info{};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = signalSemaphores;
		present_info.swapchainCount = 1;
		present_info.pSwapchains = swapChains;
		present_info.pImageIndices = &imageIndex;
		present_info.pResults = &present_result;

		GlfwVulkan::ErrorVk(vkQueuePresentKHR(Display::Get()->GetDisplayQueue(), &present_info));
		GlfwVulkan::ErrorVk(present_result);
	}
}
