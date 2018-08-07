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
		auto logicalDevice = Display::Get()->GetLogicalDevice();
		auto graphicsQueue = Display::Get()->GetGraphicsQueue();

		Display::CheckVk(vkQueueWaitIdle(graphicsQueue));

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

		auto camera = Scenes::Get()->GetCamera();
		auto stages = m_managerRender->GetStages();
		Vector4 clipPlane = Vector4(0.0f, 1.0f, 0.0f, +std::numeric_limits<float>::infinity());

		for (uint32_t stage = 0; stage < m_renderStages.size(); stage++)
		{
			// Starts Rendering.
			auto startResult = Renderer::Get()->StartRenderpass(stage);

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

						renderer->Render(*m_commandBuffer, clipPlane, *camera);
					}
				}

				if (subpass != subpassCount - 1)
				{
					Renderer::Get()->NextSubpass();
				}
			}

			// Ends Rendering.
			Renderer::Get()->EndRenderpass(stage);
		}
	}

	void Renderer::CreateRenderpass(std::vector<RenderpassCreate *> renderpassCreates)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

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

		Display::CheckVk(vkDeviceWaitIdle(logicalDevice));

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		fprintf(stdout, "Renderpass created in %fms\n", debugEnd - debugStart);
#endif
	}

	void Renderer::SetManager(IManagerRender *managerRender)
	{
		delete m_managerRender;
		m_managerRender = managerRender;
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
		commandPoolCreateInfo.queueFamilyIndex = Display::Get()->GetGraphicsFamily();
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		Display::CheckVk(vkCreateCommandPool(logicalDevice, &commandPoolCreateInfo, nullptr, &m_commandPool));

		m_commandBuffer = new CommandBuffer(false);
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

		const VkExtent2D displayExtent2D = {
			Display::Get()->GetWidth(), Display::Get()->GetHeight()
		};

		Display::CheckVk(vkQueueWaitIdle(graphicsQueue));

		if (renderStage->HasSwapchain() && !m_swapchain->IsSameExtent(displayExtent2D))
		{
#if ACID_VERBOSE
			fprintf(stdout, "Resizing swapchain: Old (%i, %i), New (%i, %i)\n", m_swapchain->GetExtent().width, m_swapchain->GetExtent().height, displayExtent2D.width, displayExtent2D.height);
#endif
			delete m_swapchain;
			m_swapchain = new Swapchain(displayExtent2D);
		}

		renderStage->Rebuild(m_swapchain);
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
			const VkResult acquireResult = vkAcquireNextImageKHR(logicalDevice, *m_swapchain->GetSwapchain(), UINT64_MAX, VK_NULL_HANDLE, m_fenceSwapchainImage, &m_activeSwapchainImage);

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

		m_commandBuffer->Begin(VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT);

		VkRect2D renderArea = {};
		renderArea.offset.x = 0;
		renderArea.offset.y = 0;
		renderArea.extent.width = renderStage->GetWidth();
		renderArea.extent.height = renderStage->GetHeight();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = renderStage->GetRenderpass()->GetRenderpass();
		renderPassBeginInfo.framebuffer = renderStage->GetActiveFramebuffer(m_activeSwapchainImage);
		renderPassBeginInfo.renderArea = renderArea;
		renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(renderStage->GetClearValues().size());
		renderPassBeginInfo.pClearValues = renderStage->GetClearValues().data();

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
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = renderStage->GetWidth();
		scissor.extent.height = renderStage->GetHeight();
		vkCmdSetScissor(m_commandBuffer->GetCommandBuffer(), 0, 1, &scissor);

		return true;
	}

	void Renderer::EndRenderpass(const uint32_t &i)
	{
		auto renderStage = GetRenderStage(i);
		auto graphicsQueue = Display::Get()->GetGraphicsQueue();
		auto presentQueue = Display::Get()->GetPresentQueue();

		vkCmdEndRenderPass(m_commandBuffer->GetCommandBuffer());

		if (!renderStage->HasSwapchain())
		{
			return;
		}

		m_commandBuffer->End();
		m_commandBuffer->Submit(false, m_semaphore);

	//	Display::CheckVk(vkQueueWaitIdle(graphicsQueue));

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

		const VkResult queuePresentResult = vkQueuePresentKHR(presentQueue, &presentInfo);

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
