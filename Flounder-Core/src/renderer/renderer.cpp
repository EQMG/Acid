#include "renderer.h"

namespace flounder
{
	renderer::renderer() :
		imodule()
	{
		m_managerRender = nullptr;

		m_swapChain = VK_NULL_HANDLE;
		m_swapChainImages = std::vector<VkImage>();
		m_swapChainImageFormat = {};
		m_swapChainExtent = {};
		m_swapChainImageViews = std::vector<VkImageView>();
		m_swapChainFramebuffers = std::vector<VkFramebuffer>();

		m_renderPass = VK_NULL_HANDLE;
		m_pipelineLayout = VK_NULL_HANDLE;
		m_graphicsPipeline = VK_NULL_HANDLE;

		m_commandPool = VK_NULL_HANDLE;
		m_commandBuffers = std::vector<VkCommandBuffer>();

		m_imageAvailableSemaphore = VK_NULL_HANDLE;
		m_renderFinishedSemaphore = VK_NULL_HANDLE;

		lastWidth = display::get()->getWidth();
		lastHeight = display::get()->getHeight();

		m_cullingBackFace = false;
		m_depthMask = true;
		m_isAlphaBlending = false;
		m_additiveBlending = false;

		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
		createSemaphores();
	}

	renderer::~renderer()
	{
		delete m_managerRender;

		// Waits for the device to finish before destroying.
		vkDeviceWaitIdle(display::get()->getVkDevice());

		cleanupSwapChain();

		vkDestroySemaphore(display::get()->getVkDevice(), m_renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(display::get()->getVkDevice(), m_imageAvailableSemaphore, nullptr);

		vkDestroyCommandPool(display::get()->getVkDevice(), m_commandPool, nullptr);
	}

	void renderer::update()
	{
		if (m_managerRender != nullptr)
		{
			m_managerRender->render();
		}
		

		int currentWidth = display::get()->getWidth();
		int currentHeight = display::get()->getHeight();

		if (currentWidth != lastWidth || currentHeight != lastHeight)
		{
			lastWidth = currentWidth;
			lastHeight = currentHeight;
			recreateSwapChain();
		}

		drawFrame();
	}

	void renderer::prepareNewRenderParse(colour *colour)
	{
		prepareNewRenderParse(colour->m_r, colour->m_g, colour->m_b, colour->m_a);
	}

	void renderer::prepareNewRenderParse(const float &r, const float &g, const float &b, const float &a)
	{
#if 0
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
		disableBlending();
		cullBackFaces(true);
		enableDepthTesting();
	}

	void renderer::cullBackFaces(const bool &cull)
	{
		if (cull && !m_cullingBackFace)
		{
#if 0
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
#endif
			m_cullingBackFace = true;
		}
		else if (!cull && m_cullingBackFace)
		{
#if 0
			glDisable(GL_CULL_FACE);
#endif
			m_cullingBackFace = false;
		}
	}

	void renderer::enableDepthTesting()
	{
#if 0
		glEnable(GL_DEPTH_TEST);
#endif
	}

	void renderer::disableDepthTesting()
	{
#if 0
		glDisable(GL_DEPTH_TEST);
#endif
	}

	void renderer::depthMask(const bool &depthMask)
	{
#if 0
		glDepthMask(depthMask);
#endif
		m_depthMask = depthMask;
	}

	void renderer::enableAlphaBlending()
	{
		if (!m_isAlphaBlending)
		{
#if 0
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
			m_isAlphaBlending = true;
			m_additiveBlending = false;
		}
	}

	void renderer::enableAdditiveBlending()
	{
		if (!m_additiveBlending)
		{
#if 0
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
#endif
			m_additiveBlending = true;
			m_isAlphaBlending = false;
		}
	}

	void renderer::disableBlending()
	{
		if (m_isAlphaBlending || m_additiveBlending)
		{
#if 0
			glDisable(GL_BLEND);
#endif
			m_isAlphaBlending = false;
			m_additiveBlending = false;
		}
	}

	void renderer::bindVAO(const int &vaoID, const int n_args, ...)
	{
#if 0
		glBindVertexArray(vaoID);
#endif

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int attribute = va_arg(ap, int);
#if 0
			glEnableVertexAttribArray(attribute);
#endif
		}

		va_end(ap);
	}

	void renderer::unbindVAO(const int n_args, ...)
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			int attribute = va_arg(ap, int);
#if 0
			glDisableVertexAttribArray(attribute);
#endif
		}

		va_end(ap);

#if 0
		glBindVertexArray(0);
#endif
	}

	void renderer::scissorEnable(const int &x, const int &y, const int &width, const int &height)
	{
#if 0
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, width, height);
#endif
	}

	void renderer::scissorDisable()
	{
#if 0
		glDisable(GL_SCISSOR_TEST);
#endif
	}

	void renderer::bindTexture(texture *texture, const int &bankID)
	{
#if 0
		glActiveTexture(GL_TEXTURE0 + bankID);

		switch (texture->getTextureType())
		{
		case texture::typeTexture2D:
			glBindTexture(GL_TEXTURE_2D, texture->getGlTexture().m_textureID);
			break;
		case texture::typeTextureCubeMap:
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getGlTexture().m_textureID);
			break;
		}
#endif
	}

	void renderer::bindTexture(const int &textureID, const int &glTarget, const int &bankID)
	{
#if 0
		glActiveTexture(GL_TEXTURE0 + bankID);
		glBindTexture(glTarget, textureID);
#endif
	}

	void renderer::bindTextureLOD(const int &textureID, const int &lodBias, const int &bankID)
	{
#if 0
		glActiveTexture(GL_TEXTURE0 + bankID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, lodBias);
		glActiveTexture(0);
#endif
	}

	void renderer::renderArrays(const int &glMode, const int &glLength)
	{
#if 0
		glDrawArrays(glMode, 0, glLength);
#endif
	}

	void renderer::renderElements(const int &glMode, const int &glType, const int &glLength)
	{
#if 0
		glDrawElements(glMode, glLength, glType, 0);
#endif
	}

	void renderer::renderInstanced(const int &glMode, const int &glLength, const int &glPrimCount)
	{
#if 0
		glDrawArraysInstanced(glMode, 0, glLength, glPrimCount);
#endif
	}

	void renderer::createSwapChain()
	{
		VkSwapChainSupportDetails swapChainSupport = querySwapChainSupport(display::get()->getVkPhysicalDevice());

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = display::get()->getVkSurface();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		VkQueueFamilyIndices indices = display::get()->findQueueFamilies(display::get()->getVkPhysicalDevice());
		uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

		if (indices.graphicsFamily != indices.presentFamily) 
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else 
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		display::vkErrorCheck(vkCreateSwapchainKHR(display::get()->getVkDevice(), &createInfo, nullptr, &m_swapChain));

		vkGetSwapchainImagesKHR(display::get()->getVkDevice(), m_swapChain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(display::get()->getVkDevice(), m_swapChain, &imageCount, m_swapChainImages.data());

		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;
	}

	void renderer::createImageViews()
	{
		m_swapChainImageViews.resize(m_swapChainImages.size());

		for (size_t i = 0; i < m_swapChainImages.size(); i++) 
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			display::vkErrorCheck(vkCreateImageView(display::get()->getVkDevice(), &createInfo, nullptr, &m_swapChainImageViews[i]));
		}
	}

	void renderer::createRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_swapChainImageFormat;
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

		display::vkErrorCheck(vkCreateRenderPass(display::get()->getVkDevice(), &renderPassInfo, nullptr, &m_renderPass));
	}

	void renderer::createGraphicsPipeline()
	{
		auto vertShaderCode = readFile("res/shaders/tests/test.vert.spv");
		auto fragShaderCode = readFile("res/shaders/tests/test.frag.spv");

		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

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
		viewport.width = (float)m_swapChainExtent.width;
		viewport.height = (float)m_swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = m_swapChainExtent;

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

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		display::vkErrorCheck(vkCreatePipelineLayout(display::get()->getVkDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = m_renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		display::vkErrorCheck(vkCreateGraphicsPipelines(display::get()->getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline));

		vkDestroyShaderModule(display::get()->getVkDevice(), fragShaderModule, nullptr);
		vkDestroyShaderModule(display::get()->getVkDevice(), vertShaderModule, nullptr);
	}

	void renderer::createFramebuffers()
	{
		m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

		for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
		{
			VkImageView attachments[] = {
				m_swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_swapChainExtent.width;
			framebufferInfo.height = m_swapChainExtent.height;
			framebufferInfo.layers = 1;

			display::vkErrorCheck(vkCreateFramebuffer(display::get()->getVkDevice(), &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]));
		}
	}

	void renderer::createCommandPool()
	{
		VkQueueFamilyIndices queueFamilyIndices = display::get()->findQueueFamilies(display::get()->getVkPhysicalDevice());

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

		display::vkErrorCheck(vkCreateCommandPool(display::get()->getVkDevice(), &poolInfo, nullptr, &m_commandPool));
	}

	void renderer::createCommandBuffers()
	{
		m_commandBuffers.resize(m_swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

		display::vkErrorCheck(vkAllocateCommandBuffers(display::get()->getVkDevice(), &allocInfo, m_commandBuffers.data()));

		for (size_t i = 0; i < m_commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_renderPass;
			renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_swapChainExtent;

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

			vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(m_commandBuffers[i]);

			display::vkErrorCheck(vkEndCommandBuffer(m_commandBuffers[i]));
		}
	}

	void renderer::recreateSwapChain()
	{
		vkDeviceWaitIdle(display::get()->getVkDevice());
		cleanupSwapChain();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandBuffers();
	}

	void renderer::cleanupSwapChain()
	{
		for (size_t i = 0; i < m_swapChainFramebuffers.size(); i++) 
		{
			vkDestroyFramebuffer(display::get()->getVkDevice(), m_swapChainFramebuffers[i], nullptr);
		}

		vkFreeCommandBuffers(display::get()->getVkDevice(), m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

		vkDestroyPipeline(display::get()->getVkDevice(), m_graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(display::get()->getVkDevice(), m_pipelineLayout, nullptr);
		vkDestroyRenderPass(display::get()->getVkDevice(), m_renderPass, nullptr);

		for (size_t i = 0; i < m_swapChainImageViews.size(); i++) 
		{
			vkDestroyImageView(display::get()->getVkDevice(), m_swapChainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(display::get()->getVkDevice(), m_swapChain, nullptr);
	}

	void renderer::createSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		display::vkErrorCheck(vkCreateSemaphore(display::get()->getVkDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore));
		display::vkErrorCheck(vkCreateSemaphore(display::get()->getVkDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore));
	}

	void renderer::drawFrame()
	{
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(display::get()->getVkDevice(), m_swapChain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
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

		display::vkErrorCheck(vkQueueSubmit(display::get()->getVkGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(display::get()->getVkPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) 
		{
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		vkQueueWaitIdle(display::get()->getVkPresentQueue());
	}

	VkShaderModule renderer::createShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		display::vkErrorCheck(vkCreateShaderModule(display::get()->getVkDevice(), &createInfo, nullptr, &shaderModule));

		return shaderModule;
	}

	std::vector<char> renderer::readFile(const std::string & filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	VkSwapChainSupportDetails renderer::querySwapChainSupport(VkPhysicalDevice device)
	{
		VkSwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, display::get()->getVkSurface(), &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, display::get()->getVkSurface(), &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, display::get()->getVkSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, display::get()->getVkSurface(), &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, display::get()->getVkSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR renderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
	{
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (VkSurfaceFormatKHR availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR renderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
	{
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (VkPresentModeKHR availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	VkExtent2D renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { display::get()->getWidth(), display::get()->getHeight() }; // WIDTH, HEIGHT

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
}
