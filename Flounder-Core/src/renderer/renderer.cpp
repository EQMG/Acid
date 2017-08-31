#include "renderer.hpp"

namespace Flounder
{
	renderer::renderer() :
		IModule()
	{
		m_managerRender = nullptr;

		m_renderPass = VK_NULL_HANDLE;
		m_pipelineLayout = VK_NULL_HANDLE;
		m_graphicsPipeline = VK_NULL_HANDLE;

		m_commandPool = VK_NULL_HANDLE;
		m_commandBuffers = std::vector<VkCommandBuffer>();

		m_imageAvailableSemaphore = VK_NULL_HANDLE;
		m_renderFinishedSemaphore = VK_NULL_HANDLE;

		m_swapChain = new swapchain();
		m_shaderTest = new shader("tests", 2,
			shadertype(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			shadertype(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		);

		lastWidth = Display::get()->getWidth();
		lastHeight = Display::get()->getHeight();

		m_swapChain->create(querySwapChainSupport(Display::get()->getVkPhysicalDevice()));
		createRenderPass();
		createGraphicsPipeline();
		m_swapChain->createFramebuffers(m_renderPass);
		createCommandPool();
		createCommandBuffers();
		createSemaphores();
	}

	renderer::~renderer()
	{
		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(Display::get()->getVkDevice());

		delete m_managerRender;
		delete m_shaderTest;
		delete m_swapChain;

		vkDestroySemaphore(Display::get()->getVkDevice(), m_renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(Display::get()->getVkDevice(), m_imageAvailableSemaphore, nullptr);

		vkDestroyCommandPool(Display::get()->getVkDevice(), m_commandPool, nullptr);
	}

	void renderer::Update()
	{
		if (m_managerRender != nullptr)
		{
			m_managerRender->render();
		}

		/*int currentWidth = display::get()->getWidth();
		int currentHeight = display::get()->getHeight();

		if (currentWidth != lastWidth || currentHeight != lastHeight)
		{
			lastWidth = currentWidth;
			lastHeight = currentHeight;
		}*/

		updateUniformBuffer();
		drawFrame();
	}

	void renderer::createRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_swapChain->getImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		Display::vkErrorCheck(vkCreateRenderPass(Display::get()->getVkDevice(), &renderPassInfo, nullptr, &m_renderPass));
	}

	void renderer::createGraphicsPipeline()
	{
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float) m_swapChain->getExtent().width;
		viewport.height = (float) m_swapChain->getExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent = m_swapChain->getExtent();

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colourBlendAttachment = {};
		colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colourBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colourBlending = {};
		colourBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colourBlending.logicOpEnable = VK_FALSE;
		colourBlending.logicOp = VK_LOGIC_OP_COPY;
		colourBlending.attachmentCount = 1;
		colourBlending.pAttachments = &colourBlendAttachment;
		colourBlending.blendConstants[0] = 0.0f;
		colourBlending.blendConstants[1] = 0.0f;
		colourBlending.blendConstants[2] = 0.0f;
		colourBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		Display::vkErrorCheck(vkCreatePipelineLayout(Display::get()->getVkDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (uint32_t)m_shaderTest->getStages()->size();
		pipelineInfo.pStages = m_shaderTest->getStages()->data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colourBlending;
		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = m_renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		Display::vkErrorCheck(vkCreateGraphicsPipelines(Display::get()->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline));
	}

	void renderer::createCommandPool()
	{
		VkQueueFamilyIndices queueFamilyIndices = Display::get()->findQueueFamilies(Display::get()->getVkPhysicalDevice());

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

		Display::vkErrorCheck(vkCreateCommandPool(Display::get()->getVkDevice(), &poolInfo, nullptr, &m_commandPool));
	}

	void renderer::createCommandBuffers()
	{
		m_commandBuffers.resize(m_swapChain->getFramebufferSize());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t) m_commandBuffers.size();

		Display::vkErrorCheck(vkAllocateCommandBuffers(Display::get()->getVkDevice(), &allocInfo, m_commandBuffers.data()));

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_renderPass;
			renderPassInfo.framebuffer = m_swapChain->getFramebuffer((uint32_t) i);
			renderPassInfo.renderArea.offset.x = 0;
			renderPassInfo.renderArea.offset.y = 0;
			renderPassInfo.renderArea.extent = m_swapChain->getExtent();

			VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

			vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(m_commandBuffers[i]);

			Display::vkErrorCheck(vkEndCommandBuffer(m_commandBuffers[i]));
		}
	}

	void renderer::createSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		Display::vkErrorCheck(vkCreateSemaphore(Display::get()->getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));
		Display::vkErrorCheck(vkCreateSemaphore(Display::get()->getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));
	}

	void renderer::updateUniformBuffer()
	{
	}

	void renderer::drawFrame()
	{
		// TODO: Fix memory leaks.

		uint32_t imageIndex;
		vkAcquireNextImageKHR(Display::get()->getVkDevice(), m_swapChain->getSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		Display::vkErrorCheck(vkQueueSubmit(Display::get()->getVkPresentQueue(), 1, &submitInfo, VK_NULL_HANDLE));

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapchains[] = {m_swapChain->getSwapchain()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueueWaitIdle(Display::get()->getVkPresentQueue());
		vkQueuePresentKHR(Display::get()->getVkPresentQueue(), &presentInfo);
	}

	VkSwapChainSupportDetails renderer::querySwapChainSupport(VkPhysicalDevice device)
	{
		VkSwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Display::get()->getVkSurface(), &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, Display::get()->getVkSurface(), &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, Display::get()->getVkSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, Display::get()->getVkSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, Display::get()->getVkSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}
}
