#include "Graphics.hpp"

#include <cstring>
#include <SPIRV/GlslangToSpv.h>

#include "Devices/Window.hpp"
#include "Subrender.hpp"

namespace acid {
Graphics::Graphics() :
	elapsedPurge(5s),
	instance(std::make_unique<Instance>()),
	physicalDevice(std::make_unique<PhysicalDevice>(instance.get())),
	surface(std::make_unique<Surface>(instance.get(), physicalDevice.get())),
	logicalDevice(std::make_unique<LogicalDevice>(instance.get(), physicalDevice.get(), surface.get())) {
	CreatePipelineCache();

	if (!glslang::InitializeProcess())
		throw std::runtime_error("Failed to initialize glslang process");
}

Graphics::~Graphics() {
	auto graphicsQueue = logicalDevice->GetGraphicsQueue();

	CheckVk(vkQueueWaitIdle(graphicsQueue));

	glslang::FinalizeProcess();

	vkDestroyPipelineCache(*logicalDevice, pipelineCache, nullptr);

	for (std::size_t i = 0; i < flightFences.size(); i++) {
		vkDestroyFence(*logicalDevice, flightFences[i], nullptr);
		vkDestroySemaphore(*logicalDevice, renderCompletes[i], nullptr);
		vkDestroySemaphore(*logicalDevice, presentCompletes[i], nullptr);
	}
	commandPools.clear ();
	commandBuffers.clear ();
	swapchain = nullptr;
	renderer = nullptr;
}

void Graphics::Update() {
	if (!renderer || Window::Get()->IsIconified()) return;

	if (!renderer->started) {
		ResetRenderStages();
		renderer->Start();
		renderer->started = true;
	}

	renderer->Update();

	auto acquireResult = swapchain->AcquireNextImage(presentCompletes[currentFrame], flightFences[currentFrame]);

	if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
		RecreateSwapchain();
		return;
	}

	if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR) {
		Log::Error("Failed to acquire swap chain image!\n");
		return;
	}

	Pipeline::Stage stage;

	for (auto &renderStage : renderer->renderStages) {
		renderStage->Update();

		if (!StartRenderpass(*renderStage))
			return;

		auto &commandBuffer = commandBuffers[swapchain->GetActiveImageIndex()];

		for (const auto &subpass : renderStage->GetSubpasses()) {
			stage.second = subpass.GetBinding();

			// Renders subpass subrender pipelines.
			renderer->subrenderHolder.RenderStage(stage, *commandBuffer);

			if (subpass.GetBinding() != renderStage->GetSubpasses().back().GetBinding())
				vkCmdNextSubpass(*commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
		}

		EndRenderpass(*renderStage);
		stage.first++;
	}

	// Purges unused command pools.
	if (elapsedPurge.GetElapsed() != 0) {
		for (auto it = commandPools.begin(); it != commandPools.end();) {
			if ((*it).second.use_count() <= 1) {
				it = commandPools.erase(it);
				continue;
			}

			++it;
		}
	}
}

std::string Graphics::StringifyResultVk(VkResult result) {
	switch (result) {
	case VK_SUCCESS:
		return "Success";
	case VK_NOT_READY:
		return "A fence or query has not yet completed";
	case VK_TIMEOUT:
		return "A wait operation has not completed in the specified time";
	case VK_EVENT_SET:
		return "An event is signaled";
	case VK_EVENT_RESET:
		return "An event is unsignaled";
	case VK_INCOMPLETE:
		return "A return array was too small for the result";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "A host memory allocation has failed";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "A device memory allocation has failed";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "Initialization of an object could not be completed for implementation-specific reasons";
	case VK_ERROR_DEVICE_LOST:
		return "The logical or physical device has been lost";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "Mapping of a memory object has failed";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "A requested layer is not present or could not be loaded";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "A requested extension is not supported";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "A requested feature is not supported";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "Too many objects of the type have already been created";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "A requested format is not supported on this device";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "A surface is no longer available";
		//case VK_ERROR_OUT_OF_POOL_MEMORY:
		//	return "A allocation failed due to having no more space in the descriptor pool";
	case VK_SUBOPTIMAL_KHR:
		return "A swapchain no longer matches the surface properties exactly, but can still be used";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "A surface has changed in such a way that it is no longer compatible with the swapchain";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "The display used by a swapchain does not use the same presentable image layout";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "A validation layer found an error";
	default:
		return "Unknown Vulkan error";
	}
}

void Graphics::CheckVk(VkResult result) {
	if (result >= 0) return;

	auto failure = StringifyResultVk(result);

	throw std::runtime_error("Vulkan error: " + failure);
}

void Graphics::CaptureScreenshot(const std::filesystem::path &filename) const {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	auto size = Window::Get()->GetSize();

	VkImage dstImage;
	VkDeviceMemory dstImageMemory;
	auto supportsBlit = Image::CopyImage(swapchain->GetActiveImage(), dstImage, dstImageMemory, surface->GetFormat().format, {size.x, size.y, 1},
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 0, 0);

	// Get layout of the image (including row pitch).
	VkImageSubresource imageSubresource = {};
	imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageSubresource.mipLevel = 0;
	imageSubresource.arrayLayer = 0;

	VkSubresourceLayout dstSubresourceLayout;
	vkGetImageSubresourceLayout(*logicalDevice, dstImage, &imageSubresource, &dstSubresourceLayout);

	Bitmap bitmap(std::make_unique<uint8_t[]>(dstSubresourceLayout.size), size);

	void *data;
	vkMapMemory(*logicalDevice, dstImageMemory, dstSubresourceLayout.offset, dstSubresourceLayout.size, 0, &data);
	std::memcpy(bitmap.GetData().get(), data, static_cast<size_t>(dstSubresourceLayout.size));
	vkUnmapMemory(*logicalDevice, dstImageMemory);

	// Frees temp image and memory.
	vkFreeMemory(*logicalDevice, dstImageMemory, nullptr);
	vkDestroyImage(*logicalDevice, dstImage, nullptr);

	// Writes the screenshot bitmap to the file.
	bitmap.Write(filename);

#if defined(ACID_DEBUG)
	Log::Out("Screenshot ", filename, " created in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

RenderStage *Graphics::GetRenderStage(uint32_t index) const {
	if (renderer)
		return renderer->GetRenderStage(index);
	return nullptr;
}

const Descriptor *Graphics::GetAttachment(const std::string &name) const {
	if (auto it = attachments.find(name); it != attachments.end())
		return it->second;
	return nullptr;
}

const std::shared_ptr<CommandPool> &Graphics::GetCommandPool(const std::thread::id &threadId) {
	if (auto it = commandPools.find(threadId); it != commandPools.end())
		return it->second;
	// TODO: Cleanup and fix crashes
	return commandPools.emplace(threadId, std::make_shared<CommandPool>(threadId)).first->second;
}

void Graphics::CreatePipelineCache() {
	VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
	pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	CheckVk(vkCreatePipelineCache(*logicalDevice, &pipelineCacheCreateInfo, nullptr, &pipelineCache));
}

void Graphics::ResetRenderStages() {
	RecreateSwapchain();

	if (flightFences.size() != swapchain->GetImageCount())
		RecreateCommandBuffers();

	for (const auto &renderStage : renderer->renderStages)
		renderStage->Rebuild(*swapchain);

	RecreateAttachmentsMap();
}

void Graphics::RecreateSwapchain() {
	vkDeviceWaitIdle(*logicalDevice);

	VkExtent2D displayExtent = {Window::Get()->GetSize().x, Window::Get()->GetSize().y};
#if defined(ACID_DEBUG)
	if (swapchain)
		Log::Out("Recreating swapchain old (", swapchain->GetExtent().width, ", ", swapchain->GetExtent().height, ") new (", displayExtent.width, ", ", displayExtent.height, ")\n");
#endif
	swapchain = std::make_unique<Swapchain>(displayExtent, swapchain.get());
	RecreateCommandBuffers();
}

void Graphics::RecreateCommandBuffers() {
	for (std::size_t i = 0; i < flightFences.size(); i++) {
		vkDestroyFence(*logicalDevice, flightFences[i], nullptr);
		vkDestroySemaphore(*logicalDevice, renderCompletes[i], nullptr);
		vkDestroySemaphore(*logicalDevice, presentCompletes[i], nullptr);
	}

	presentCompletes.resize(swapchain->GetImageCount());
	renderCompletes.resize(swapchain->GetImageCount());
	flightFences.resize(swapchain->GetImageCount());
	commandBuffers.resize(swapchain->GetImageCount());

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (std::size_t i = 0; i < flightFences.size(); i++) {
		CheckVk(vkCreateSemaphore(*logicalDevice, &semaphoreCreateInfo, nullptr, &presentCompletes[i]));

		CheckVk(vkCreateSemaphore(*logicalDevice, &semaphoreCreateInfo, nullptr, &renderCompletes[i]));

		CheckVk(vkCreateFence(*logicalDevice, &fenceCreateInfo, nullptr, &flightFences[i]));

		commandBuffers[i] = std::make_unique<CommandBuffer>(false);
	}
}

void Graphics::RecreatePass(RenderStage &renderStage) {
	auto graphicsQueue = logicalDevice->GetGraphicsQueue();

	VkExtent2D displayExtent = {Window::Get()->GetSize().x, Window::Get()->GetSize().y};

	CheckVk(vkQueueWaitIdle(graphicsQueue));

	if (renderStage.HasSwapchain() && (framebufferResized || !swapchain->IsSameExtent(displayExtent)))
		RecreateSwapchain();

	renderStage.Rebuild(*swapchain);
	RecreateAttachmentsMap(); // TODO: Maybe not recreate on a single change.
}

void Graphics::RecreateAttachmentsMap() {
	attachments.clear();

	for (const auto &renderStage : renderer->renderStages)
		attachments.insert(renderStage->descriptors.begin(), renderStage->descriptors.end());
}

bool Graphics::StartRenderpass(RenderStage &renderStage) {
	if (renderStage.IsOutOfDate()) {
		RecreatePass(renderStage);
		return false;
	}

	auto &commandBuffer = commandBuffers[swapchain->GetActiveImageIndex()];

	if (!commandBuffer->IsRunning())
		commandBuffer->Begin(VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

	VkRect2D renderArea = {};
	renderArea.offset = {renderStage.GetRenderArea().GetOffset().x, renderStage.GetRenderArea().GetOffset().y};
	renderArea.extent = {renderStage.GetRenderArea().GetExtent().x, renderStage.GetRenderArea().GetExtent().y};

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(renderArea.extent.width);
	viewport.height = static_cast<float>(renderArea.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(*commandBuffer, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = renderArea.offset;
	scissor.extent = renderArea.extent;
	vkCmdSetScissor(*commandBuffer, 0, 1, &scissor);

	auto clearValues = renderStage.GetClearValues();

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = *renderStage.GetRenderpass();
	renderPassBeginInfo.framebuffer = renderStage.GetActiveFramebuffer(swapchain->GetActiveImageIndex());
	renderPassBeginInfo.renderArea = renderArea;
	renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassBeginInfo.pClearValues = clearValues.data();
	vkCmdBeginRenderPass(*commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	return true;
}

void Graphics::EndRenderpass(RenderStage &renderStage) {
	auto presentQueue = logicalDevice->GetPresentQueue();
	auto &commandBuffer = commandBuffers[swapchain->GetActiveImageIndex()];

	vkCmdEndRenderPass(*commandBuffer);

	if (!renderStage.HasSwapchain())
		return;

	commandBuffer->End();
	commandBuffer->Submit(presentCompletes[currentFrame], renderCompletes[currentFrame], flightFences[currentFrame]);

	auto presentResult = swapchain->QueuePresent(presentQueue, renderCompletes[currentFrame]);
	if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR) { // || framebufferResized
		framebufferResized = true; // false
		//RecreateSwapchain();
	} else if (presentResult != VK_SUCCESS) {
		CheckVk(presentResult);
		Log::Error("Failed to present swap chain image!\n");
	}

	currentFrame = (currentFrame + 1) % swapchain->GetImageCount();
}
}
