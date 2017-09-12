#include "Renderer.hpp"

namespace Flounder
{
	Renderer::Renderer() :
		IModule()
	{
		m_managerRender = nullptr;

		m_renderPass = VK_NULL_HANDLE;
		m_pipelineLayout = VK_NULL_HANDLE;
		m_graphicsPipeline = VK_NULL_HANDLE;

		m_swapchain = new Swapchain();

		m_commandPool = new CommandPool();
		m_commandPoolTransfer = new CommandPool();

		m_commandBuffers = std::vector<VkCommandBuffer>();

		m_imageAvailableSemaphore = VK_NULL_HANDLE;
		m_renderFinishedSemaphore = VK_NULL_HANDLE;

		m_shaderTest = new Shader("tests", 2,
			ShaderType(VK_SHADER_STAGE_VERTEX_BIT, "res/shaders/tests/test.vert.spv"),
			ShaderType(VK_SHADER_STAGE_FRAGMENT_BIT, "res/shaders/tests/test.frag.spv")
		);
		m_vertexBuffer = new VertexBuffer();

		lastWidth = Display::Get()->GetWidth();
		lastHeight = Display::Get()->GetHeight();

		VkDevice logicalDevice = Display::Get()->GetVkDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetVkPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetVkSurface();
		VkQueue transferQueue = Display::Get()->GetVkTransferQueue();
		GLFWwindow *window = Display::Get()->GetGlfwWindow();

		m_swapchain->Create(&logicalDevice, &physicalDevice, &surface, window);
		CreateRenderPass();
		CreateGraphicsPipeline();
		m_swapchain->CreateFramebuffers(&logicalDevice, &m_renderPass);
		QueueFamilyIndices indices = QueueFamily::FindQueueFamilies(&physicalDevice, &surface);
		m_commandPool->Create(&logicalDevice, &physicalDevice, &surface, indices.graphicsFamily);
		m_commandPoolTransfer->Create(&logicalDevice, &physicalDevice, &surface, indices.transferFamily, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
		m_vertexBuffer->Create(&logicalDevice, &physicalDevice, &surface, m_commandPoolTransfer->GetCommandPool(), &transferQueue);
		CreateCommandBuffers();
		CreateSemaphores();
	}

	Renderer::~Renderer()
	{
		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(Display::Get()->GetVkDevice());

		delete m_managerRender;
		delete m_shaderTest;
		delete m_swapchain;
		delete m_vertexBuffer;

		vkDestroySemaphore(Display::Get()->GetVkDevice(), m_renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(Display::Get()->GetVkDevice(), m_imageAvailableSemaphore, nullptr);

	//	vkDestroyCommandPool(Display::Get()->GetVkDevice(), m_commandPool, nullptr);
		m_commandPool->Cleanup();
	}

	void Renderer::Update()
	{
		if (m_managerRender != nullptr)
		{
			m_managerRender->Render();
		}

		/*int currentWidth = Display::Get()->getWidth();
		int currentHeight = Display::Get()->getHeight();

		if (currentWidth != lastWidth || currentHeight != lastHeight)
		{
			lastWidth = currentWidth;
			lastHeight = currentHeight;
		}*/

		UpdateUniformBuffer();
		DrawFrame();
	}

	void Renderer::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_swapchain->GetImageFormat();
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

		GlfwVulkan::ErrorCheck(vkCreateRenderPass(Display::Get()->GetVkDevice(), &renderPassInfo, nullptr, &m_renderPass));
	}

	void Renderer::CreateGraphicsPipeline()
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
		viewport.width = static_cast<float>(m_swapchain->GetExtent().width);
		viewport.height = static_cast<float>(m_swapchain->GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent = m_swapchain->GetExtent();

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

		GlfwVulkan::ErrorCheck(vkCreatePipelineLayout(Display::Get()->GetVkDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = (uint32_t) m_shaderTest->GetStages()->size();
		pipelineInfo.pStages = m_shaderTest->GetStages()->data();
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

		GlfwVulkan::ErrorCheck(vkCreateGraphicsPipelines(Display::Get()->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline));
	}

	void Renderer::CreateCommandBuffers()
	{
		m_commandBuffers.resize(m_swapchain->GetFramebufferSize());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *m_commandPool->GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

		GlfwVulkan::ErrorCheck(vkAllocateCommandBuffers(Display::Get()->GetVkDevice(), &allocInfo, m_commandBuffers.data()));

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_renderPass;
			renderPassInfo.framebuffer = m_swapchain->GetFramebuffer(static_cast<uint32_t>(i));
			renderPassInfo.renderArea.offset.x = 0;
			renderPassInfo.renderArea.offset.y = 0;
			renderPassInfo.renderArea.extent = m_swapchain->GetExtent();

			VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

			VkBuffer vertexBuffers[] = { m_vertexBuffer->GetVertexBuffer()->GetBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdDraw(m_commandBuffers[i], m_vertexBuffer->GetVerticesSize(), 1, 0, 0);

			vkCmdEndRenderPass(m_commandBuffers[i]);

			GlfwVulkan::ErrorCheck(vkEndCommandBuffer(m_commandBuffers[i]));
		}
	}

	void Renderer::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		GlfwVulkan::ErrorCheck(vkCreateSemaphore(Display::Get()->GetVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));
		GlfwVulkan::ErrorCheck(vkCreateSemaphore(Display::Get()->GetVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));
	}

	void Renderer::UpdateUniformBuffer()
	{
	}

	void Renderer::DrawFrame()
	{
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(Display::Get()->GetVkDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

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

		GlfwVulkan::ErrorCheck(vkQueueSubmit(Display::Get()->GetVkTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE));

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapchains[] = { m_swapchain->GetSwapchain()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueueWaitIdle(Display::Get()->GetVkTransferQueue());
		vkQueuePresentKHR(Display::Get()->GetVkTransferQueue(), &presentInfo);
	}

	VkSwapChainSupportDetails Renderer::QuerySwapChainSupport(VkPhysicalDevice device)
	{
		VkSwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Display::Get()->GetVkSurface(), &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, Display::Get()->GetVkSurface(), &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, Display::Get()->GetVkSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, Display::Get()->GetVkSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, Display::Get()->GetVkSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}
}
