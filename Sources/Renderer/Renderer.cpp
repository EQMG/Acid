#include "Renderer.hpp"

#include "Scenes/Scenes.hpp"
#include "IRenderer.hpp"

namespace acid
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
		m_commandBuffer(nullptr)
	{
		CreateFences();
		CreateCommandPool();
		CreatePipelineCache();
	}

	Renderer::~Renderer()
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto queueGraphics = Display::Get()->GetVkQueueGraphics();

		vkQueueWaitIdle(queueGraphics);

		delete m_managerRender;

		for (auto &renderStage : m_renderStages)
		{
			delete renderStage;
		}

		delete m_swapchain;
		delete m_commandBuffer;

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

		m_managerRender->Update();

		auto commandBuffer = Renderer::Get()->GetCommandBuffer();
		auto camera = Scenes::Get()->GetCamera();
		auto stages = m_managerRender->GetStages();
		Vector4 clipPlane = Vector4(0.0f, 1.0f, 0.0f, +std::numeric_limits<float>::infinity());

		for (uint32_t stage = 0; stage < m_renderStages.size(); stage++)
		{
			// Starts Rendering.
			auto startResult = Renderer::Get()->StartRenderpass(*commandBuffer, stage);

			if (!startResult)
			{
				return;
			}

			// Renders subpasses.
			uint32_t subpassCount = Renderer::Get()->GetRenderStage(stage)->SubpassCount();

			for (uint32_t subpass = 0; subpass < subpassCount; subpass++)
			{
				float key = m_managerRender->GetStageKey(stage, subpass);
				auto renderers = stages.find(key);

				if (renderers != stages.end())
				{
					for (auto &renderer : (*renderers).second)
					{
						if (!renderer->IsEnabled())
						{
							continue;
						}

						renderer->Render(*commandBuffer, clipPlane, *camera);
					}
				}

				if (subpass != subpassCount - 1)
				{
					Renderer::Get()->NextSubpass(*commandBuffer);
				}
			}

			// Ends Rendering.
			Renderer::Get()->EndRenderpass(*commandBuffer, stage);
		}
	}

	void Renderer::CreateRenderpass(std::vector<RenderpassCreate *> renderpassCreates)
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
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
			m_renderStages.emplace_back(renderStage);
		}

		vkDeviceWaitIdle(Display::Get()->GetVkLogicalDevice());
		vkQueueWaitIdle(Display::Get()->GetVkQueueGraphics());

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Renderpass created in %fms\n", debugEnd - debugStart);
#endif
	}

	bool Renderer::StartRenderpass(const CommandBuffer &commandBuffer, const uint32_t &i)
	{
		auto renderStage = GetRenderStage(i);

		if (renderStage->IsOutOfDate(m_swapchain->GetVkExtent()))
		{
			RecreatePass(i);
			return false;
		}

		auto logicalDevice = Display::Get()->GetVkLogicalDevice();
		auto queueGraphics = Display::Get()->GetVkQueueGraphics();

		Display::CheckVk(vkQueueWaitIdle(queueGraphics));

		if (renderStage->HasSwapchain())
		{
			const VkResult acquireResult = vkAcquireNextImageKHR(logicalDevice, *m_swapchain->GetVkSwapchain(), UINT64_MAX, VK_NULL_HANDLE, m_fenceSwapchainImage, &m_activeSwapchainImage);

			if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
			{
				RecreatePass(i);
				return false;
			}

			if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
			{
				throw std::runtime_error("Renderer failed to acquire swapchain image!");
			}

			Display::CheckVk(vkWaitForFences(logicalDevice, 1, &m_fenceSwapchainImage, VK_TRUE, UINT64_MAX));

			Display::CheckVk(vkResetFences(logicalDevice, 1, &m_fenceSwapchainImage));
		}

		VkCommandBufferBeginInfo commandBufferBeginInfo = {};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

		Display::CheckVk(vkBeginCommandBuffer(commandBuffer.GetVkCommandBuffer(), &commandBufferBeginInfo));

		VkRect2D renderArea = {};
		renderArea.offset.x = 0;
		renderArea.offset.y = 0;
		renderArea.extent.width = renderStage->GetWidth();
		renderArea.extent.height = renderStage->GetHeight();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderStage->GetRenderpass()->GetVkRenderpass();
		renderPassBeginInfo.framebuffer = renderStage->GetActiveFramebuffer(m_activeSwapchainImage);
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(renderStage->GetClearValues().size());
		renderPassBeginInfo.pClearValues = renderStage->GetClearValues().data();

		vkCmdBeginRenderPass(commandBuffer.GetVkCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(renderStage->GetWidth());
		viewport.height = static_cast<float>(renderStage->GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer.GetVkCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = renderStage->GetWidth();
		scissor.extent.height = renderStage->GetHeight();
		vkCmdSetScissor(commandBuffer.GetVkCommandBuffer(), 0, 1, &scissor);

		return true;
	}

	void Renderer::EndRenderpass(const CommandBuffer &commandBuffer, const uint32_t &i)
	{
		auto renderStage = GetRenderStage(i);
		auto queueGraphics = Display::Get()->GetVkQueueGraphics();
		auto presentQueue = Display::Get()->GetVkQueuePresent();

		vkCmdEndRenderPass(commandBuffer.GetVkCommandBuffer());
		Display::CheckVk(vkEndCommandBuffer(commandBuffer.GetVkCommandBuffer()));

		VkCommandBuffer commandBuffers = commandBuffer.GetVkCommandBuffer();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers;

		if (renderStage->HasSwapchain())
		{
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &m_semaphore;
		}

		const VkResult queueSubmitResult = vkQueueSubmit(queueGraphics, 1, &submitInfo, VK_NULL_HANDLE);

		if (queueSubmitResult != VK_SUCCESS)
		{
			throw std::runtime_error("Renderer failed to acquire swapchain image!");
		}

		Display::CheckVk(vkQueueWaitIdle(queueGraphics));

		if (!renderStage->HasSwapchain())
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
		presentInfo.pSwapchains = m_swapchain->GetVkSwapchain();
		presentInfo.pImageIndices = &m_activeSwapchainImage;
		presentInfo.pResults = &result;

		const VkResult queuePresentResult = vkQueuePresentKHR(presentQueue, &presentInfo);

		if (queuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || queuePresentResult == VK_SUBOPTIMAL_KHR)
		{
			RecreatePass(i);
			return;
		}

		Display::CheckVk(result);
		vkQueueWaitIdle(queueGraphics);
	}

	void Renderer::NextSubpass(const CommandBuffer &commandBuffer)
	{
		vkCmdNextSubpass(commandBuffer.GetVkCommandBuffer(), VK_SUBPASS_CONTENTS_INLINE);
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

//#ifdef ACID_BUILD_WINDOWS
//		MessageBox(nullptr, "Couldn't find proper memory type!", "Vulkan Error", 0);
//#endif
		throw std::runtime_error("Vulkan runtime error, couldn't find proper memory type!");
		return UINT32_MAX;
	}

	void Renderer::SetManager(IManagerRender *managerRender)
	{
		delete m_managerRender;
		m_managerRender = managerRender;
	}

	void Renderer::CreateFences()
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, &m_fenceSwapchainImage);
	}

	void Renderer::CreateCommandPool()
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		Display::CheckVk(vkCreateSemaphore(logicalDevice, &semaphoreCreateInfo, nullptr, &m_semaphore));

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = Display::Get()->GetVkQueueIndices().GetGraphicsFamily();
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		Display::CheckVk(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &m_commandPool));

		m_commandBuffer = new CommandBuffer(false);
	}

	void Renderer::CreatePipelineCache()
	{
		auto logicalDevice = Display::Get()->GetVkLogicalDevice();

		VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
		pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		Display::CheckVk(vkCreatePipelineCache(logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
	}

	void Renderer::RecreatePass(const uint32_t &i)
	{
		auto queueGraphics = Display::Get()->GetVkQueueGraphics();
		auto renderStage = GetRenderStage(i);

		const VkExtent2D displayExtent2D = {
			Display::Get()->GetWidth(), Display::Get()->GetHeight()
		};

		Display::CheckVk(vkQueueWaitIdle(queueGraphics));

		if (renderStage->HasSwapchain() && !m_swapchain->IsSameExtent(displayExtent2D))
		{
#if ACID_VERBOSE
			fprintf(stdout, "Resizing swapchain: Old (%i, %i), New (%i, %i)\n", m_swapchain->GetVkExtent().width, m_swapchain->GetVkExtent().height, displayExtent2D.width, displayExtent2D.height);
#endif
			delete m_swapchain;
			m_swapchain = new Swapchain(displayExtent2D);
		}

		renderStage->Rebuild(m_swapchain);
	}
}
