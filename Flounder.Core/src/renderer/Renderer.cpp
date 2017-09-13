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
		const std::vector<Vertex> verticesTriangle =
		{
			{ Vector3(0.0f, -0.5f, 0.0f), Colour(1.0f, 0.0f, 0.0f) },
			{ Vector3(0.5f, 0.5f, 0.0f), Colour(0.0f, 1.0f, 0.0f) },
			{ Vector3(-0.5f, 0.5f, 0.0f), Colour(0.0f, 0.0f, 1.0f) }
		};
		m_vertexBuffer = new VertexBuffer(verticesTriangle);

		lastWidth = Display::Get()->GetWidth();
		lastHeight = Display::Get()->GetHeight();

		VkDevice logicalDevice = Display::Get()->GetVkDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetVkPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetVkSurface();
		VkQueue transferQueue = Display::Get()->GetVkTransferQueue();
		GLFWwindow *window = Display::Get()->GetGlfwWindow();

		m_swapchain->Create(&logicalDevice, &physicalDevice, &surface, window);
		CreateRenderPass();
		m_shaderTest->Create(&logicalDevice);
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
		VkDevice logicalDevice = Display::Get()->GetVkDevice();

		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(Display::Get()->GetVkDevice());

		delete m_managerRender;
		m_shaderTest->Cleanup(&logicalDevice);
		delete m_shaderTest;
		m_swapchain->CleanupFrameBuffers(&logicalDevice);
		m_swapchain->Cleanup(&logicalDevice);
		delete m_swapchain;
		m_vertexBuffer->Cleanup(&logicalDevice);
		delete m_vertexBuffer;

		vkDestroySemaphore(Display::Get()->GetVkDevice(), m_renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(Display::Get()->GetVkDevice(), m_imageAvailableSemaphore, nullptr);

		m_commandPool->Cleanup(&logicalDevice);
		delete m_commandPool;
	}

	void Renderer::Update()
	{
		if (m_managerRender != nullptr)
		{
			m_managerRender->Render();
		}

		int currentWidth = Display::Get()->GetWidth();
		int currentHeight = Display::Get()->GetHeight();

		if (currentWidth != lastWidth || currentHeight != lastHeight)
		{
			lastWidth = currentWidth;
			lastHeight = currentHeight;
		}

		DrawFrame();
	}

	void Renderer::CreateRenderPass()
	{
		// Colour attachment.
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_swapchain->GetImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Reference to color attachment for subpass.
		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Subpass struct.
		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorAttachmentRef;

		// Subpass dependancy.
		VkSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dstSubpass = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.srcAccessMask = 0;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &subpassDependency;

		GlfwVulkan::ErrorCheck(vkCreateRenderPass(Display::Get()->GetVkDevice(), &renderPassInfo, nullptr, &m_renderPass));
	}

	void Renderer::CreateGraphicsPipeline()
	{
		// Vertex input struct.
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		// Input assembly struct.
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Viewport struct.
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_swapchain->GetExtent().width);
		viewport.height = static_cast<float>(m_swapchain->GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Scissor rect struct.
		VkRect2D scissor = {};
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent = m_swapchain->GetExtent();

		// Viewport state struct.
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// Rasterizer.
		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		// Multisampling struct.
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// Colour blending attachment struct.
		VkPipelineColorBlendAttachmentState colourBlendAttachment = {};
		colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colourBlendAttachment.blendEnable = VK_FALSE;

		// Colour blend struct.
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

		// Dynamic states struct.
		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_LINE_WIDTH };
		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;

		// Pipeline layout struct.
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		// Creates the graphics pipeline layout.
		GlfwVulkan::ErrorCheck(vkCreatePipelineLayout(Display::Get()->GetVkDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

		// Pipeline info struct.
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(m_shaderTest->GetStages()->size());
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

		// Create the graphics pipeline.
		GlfwVulkan::ErrorCheck(vkCreateGraphicsPipelines(Display::Get()->GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline));
	}

	void Renderer::CreateCommandBuffers()
	{

		m_commandBuffers.resize(m_swapchain->GetFramebufferSize());//swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *m_commandPool->GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

		if (vkAllocateCommandBuffers(Display::Get()->GetVkDevice(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
		else
		{
			std::cout << "Command buffers allocated successfully" << std::endl;
		}

		for (size_t i = 0; i < m_commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_renderPass;
			renderPassInfo.framebuffer = m_swapchain->GetFramebuffer(static_cast<uint32_t>(i));//swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_swapchain->GetExtent();//swapChainExtent;
			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
			VkBuffer vertexBuffers[] = { *m_vertexBuffer->GetBuffer() };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdDraw(m_commandBuffers[i], static_cast<uint32_t>(m_vertexBuffer->GetVerticesSize()), 1, 0, 0);

			vkCmdEndRenderPass(m_commandBuffers[i]);

			if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		std::cout << "Command buffers recorded successfully" << std::endl;
	}

	void Renderer::CreateSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		GlfwVulkan::ErrorCheck(vkCreateSemaphore(Display::Get()->GetVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));
		GlfwVulkan::ErrorCheck(vkCreateSemaphore(Display::Get()->GetVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));
	}

	void Renderer::DrawFrame()
	{
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(Display::Get()->GetVkDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}


		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];
		VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(Display::Get()->GetVkDisplayQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		//if you have a success output here, it fills up the output log

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapchain->GetSwapchain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueueWaitIdle(Display::Get()->GetVkDisplayQueue());
		result = vkQueuePresentKHR(Display::Get()->GetVkDisplayQueue(), &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	void Renderer::RecreateSwapChain()
	{
		VkDevice logicalDevice = Display::Get()->GetVkDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetVkPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetVkSurface();
		GLFWwindow *window = Display::Get()->GetGlfwWindow();

		vkDeviceWaitIdle(logicalDevice);

		CleanupSwapChain();

		m_swapchain->Create(&logicalDevice, &physicalDevice, &surface, window);
		CreateRenderPass();
		CreateGraphicsPipeline();
		m_swapchain->CreateFramebuffers(&logicalDevice, &m_renderPass);
		CreateCommandBuffers();
	}

	void Renderer::CleanupSwapChain()
	{
		VkDevice logicalDevice = Display::Get()->GetVkDevice();
		VkPhysicalDevice physicalDevice = Display::Get()->GetVkPhysicalDevice();
		VkSurfaceKHR surface = Display::Get()->GetVkSurface();
		GLFWwindow *window = Display::Get()->GetGlfwWindow();

		m_swapchain->CleanupFrameBuffers(&logicalDevice);

		vkFreeCommandBuffers(logicalDevice, *m_commandPool->GetCommandPool(), static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

		vkDestroyPipeline(logicalDevice, m_graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(logicalDevice, m_pipelineLayout, nullptr);
		vkDestroyRenderPass(logicalDevice, m_renderPass, nullptr);

		m_swapchain->Cleanup(&logicalDevice);
	}
}
