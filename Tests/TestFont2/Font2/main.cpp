#if 1
#include <memory>
#include <array>
#include <cassert>
#include <vector>
#include <map>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <Maths/Maths.hpp>
#include <Maths/Colour.hpp>
#include "Outline.hpp"

using namespace acid;

#define VK_DESTROY(func, dev, obj) func(dev, obj, NULL), obj = NULL
#define VK_CHECK(r) do { VkResult res = (r); if (res != VK_SUCCESS) exit(1); } while (0)

#define WIDTH 1280
#define HEIGHT 720

#define MAX_VISIBLE_GLYPHS 4096

struct CellInfo
{
	uint32_t pointOffset;
	uint32_t cellOffset;
	uint32_t cellCountX;
	uint32_t cellCountY;
};

struct GlyphInstance
{
	Rect rect;
	uint32_t glyphIndex;
	float sharpness;
	Colour colour;
};

struct HostGlyphInfo
{
	Rect bbox;
	float advance;
};

struct DeviceGlyphInfo
{
	Rect bbox;
//	Rect cbox;
	CellInfo cellInfo;
};

struct Render
{
	uint32_t frame;
	uint32_t fpsTemp;
	uint32_t fps;
	float time;
	float deltaTime;

	Vector2 mousePos;
	Vector2 oldMousePos;
	Vector2 canvasOffset;
	float canvasScale;
	float targetCanvasScale;

	std::map<wchar_t, uint32_t> charmap;
	std::vector<HostGlyphInfo> glyphInfos;

	std::unique_ptr<char[]> glyphData;
	uint32_t glyphDataSize;
	uint32_t glyphInfoSize;
	uint32_t glyphCellsSize;
	uint32_t glyphPointsSize;
	uint32_t glyphInfoOffset;
	uint32_t glyphCellsOffset;
	uint32_t glyphPointsOffset;

	GlyphInstance *glyphInstances;
	uint32_t glyphInstanceCount;

	GLFWwindow *window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceMemoryProperties memoryProperties;
	VkPhysicalDeviceProperties deviceProperties;
	uint32_t queueFamilyGraphics;
	uint32_t queueFamilyPresent;
	VkDevice device;
	VkQueue queueGraphics;
	VkQueue queuePresent;
	VkSurfaceKHR surface;
	VkFormat format;
	VkColorSpaceKHR colorSpace;
	VkPresentModeKHR presentMode;
	VkSwapchainKHR swapchain;
	VkExtent2D swapchainExtent;
	uint32_t imageIndex;
	uint32_t imageCount;
	std::vector<VkImage> images;
	std::vector<VkImageView> imageViews;
	VkRenderPass renderPass;
	std::vector<VkFramebuffer> framebuffers;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	VkDeviceMemory storageBufferMemory;
	VkBuffer storageBuffer;
	VkDeviceMemory instanceBufferMemory;
	VkBuffer instanceBuffer;
	VkDeviceMemory instanceStagingBufferMemory;
	VkBuffer instanceStagingBuffer;
	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;
	VkFence commandBufferFence;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;
	VkDescriptorSetLayout setLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;
};

static uint32_t AlignUint32(const uint32_t &value, const uint32_t &alignment)
{
	return (value + alignment - 1) / alignment * alignment;
}

static void LoadFont(const std::string &filename, Render *r)
{
	FT_Library library;
	FT_CHECK(FT_Init_FreeType(&library));

	FT_Face face;
	FT_CHECK(FT_New_Face(library, filename.c_str(), 0, &face)); // TODO: FT_New_Memory_Face
	FT_CHECK(FT_Set_Char_Size(face, 0, 1000 * 64, 96, 96));

	uint32_t totalPoints = 0;
	uint32_t totalCells = 0;
	printf("Glyph Count: %i\n", face->num_glyphs);

	r->charmap = std::map<wchar_t, uint32_t>();
	r->glyphInfos = std::vector<HostGlyphInfo>(face->num_glyphs);
	std::vector<Outline> outlines(face->num_glyphs);

	FT_UInt glyphIndex;
	FT_ULong charcode = FT_Get_First_Char(face, &glyphIndex);
	uint32_t i = 0;

	while (glyphIndex != 0)
	{
		FT_CHECK(FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_HINTING));
		printf("%i(%i) = %c\n", i, glyphIndex, charcode);

		r->charmap.emplace(charcode, i);
		HostGlyphInfo *hgi = &r->glyphInfos[i];
		Outline *o = &outlines[i];

		OutlineConvert(&face->glyph->outline, o);

		hgi->bbox = o->bbox;
		hgi->advance = face->glyph->metrics.horiAdvance / 64.0f;

		totalPoints += o->points.size();
		totalCells += o->cells.size();

		charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
		i++;
	}

//	printf("Glyph count: %i\n", i);

	r->glyphInfoSize = sizeof(DeviceGlyphInfo) * r->glyphInfos.size();
	r->glyphCellsSize = sizeof(uint32_t) * totalCells;
	r->glyphPointsSize = sizeof(Vector2) * totalPoints;

	uint32_t alignment = r->deviceProperties.limits.minStorageBufferOffsetAlignment;
	r->glyphInfoOffset = 0;
	r->glyphCellsOffset = AlignUint32(r->glyphInfoSize, alignment);
	r->glyphPointsOffset = AlignUint32(r->glyphInfoSize + r->glyphCellsSize, alignment);

	r->glyphDataSize = r->glyphPointsOffset + r->glyphPointsSize;
	r->glyphData = std::make_unique<char[]>(r->glyphDataSize);

	auto deviceGlyphInfos = reinterpret_cast<DeviceGlyphInfo*>(r->glyphData.get() + r->glyphInfoOffset);
	auto cells = reinterpret_cast<uint32_t*>(r->glyphData.get() + r->glyphCellsOffset);
	auto points = reinterpret_cast<Vector2*>(r->glyphData.get() + r->glyphPointsOffset);

	uint32_t pointOffset = 0;
	uint32_t cellOffset = 0;

	for (uint32_t i = 0; i < r->glyphInfos.size(); i++)
	{
		Outline *o = &outlines[i];
		DeviceGlyphInfo *dgi = &deviceGlyphInfos[i];

		dgi->cellInfo.cellCountX = o->cellCountX;
		dgi->cellInfo.cellCountY = o->cellCountY;
		dgi->cellInfo.pointOffset = pointOffset;
		dgi->cellInfo.cellOffset = cellOffset;
		dgi->bbox = o->bbox;

		memcpy(cells + cellOffset, o->cells.data(), sizeof(uint32_t) * o->cells.size());
		memcpy(points + pointOffset, o->points.data(), sizeof(Vector2) * o->points.size());

	//	OutlineU16Points(o, &dgi->cbox, reinterpret_cast<PointU16 *>(reinterpret_cast<char *>(points) + pointOffset));

		pointOffset += o->points.size();
		cellOffset += o->cells.size();
	}

	assert(pointOffset == totalPoints);
	assert(cellOffset == totalCells);

	FT_CHECK(FT_Done_Face(face));
	FT_CHECK(FT_Done_FreeType(library));
}

static void CreateInstance(Render *r)
{
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = nullptr;
	applicationInfo.pApplicationName = "Font Renderer";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	uint32_t requiredInstanceExtensionCount;
	const char **requiredInstanceExtensions = glfwGetRequiredInstanceExtensions(&requiredInstanceExtensionCount);

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;
	instanceCreateInfo.enabledExtensionCount = requiredInstanceExtensionCount;
	instanceCreateInfo.ppEnabledExtensionNames = requiredInstanceExtensions;

	VK_CHECK(vkCreateInstance(&instanceCreateInfo, NULL, &r->instance));
}

static void CreateSurface(Render *r)
{
	VK_CHECK(glfwCreateWindowSurface(r->instance, r->window, NULL, &r->surface));
}

static void PickPhysicalDevice(Render *r)
{
	uint32_t physicalDeviceCount;
	VK_CHECK(vkEnumeratePhysicalDevices(r->instance, &physicalDeviceCount, NULL));
	assert(physicalDeviceCount > 0);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	VK_CHECK(vkEnumeratePhysicalDevices(r->instance, &physicalDeviceCount, physicalDevices.data()));

	r->physicalDevice = physicalDevices[0];

	vkGetPhysicalDeviceMemoryProperties(r->physicalDevice, &r->memoryProperties);
	vkGetPhysicalDeviceProperties(r->physicalDevice, &r->deviceProperties);
}

static void CreateDevice(Render *r)
{
	char const *extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = nullptr;
	deviceCreateInfo.enabledExtensionCount = 1;
	deviceCreateInfo.ppEnabledExtensionNames = extensions;

	float queuePriority = 1.0f;

	if (r->queueFamilyGraphics == r->queueFamilyPresent)
	{
		VkDeviceQueueCreateInfo queue_info = {};
		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.pNext = nullptr;
		queue_info.queueFamilyIndex = r->queueFamilyGraphics;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = &queuePriority;

		deviceCreateInfo.pQueueCreateInfos = &queue_info;
		deviceCreateInfo.queueCreateInfoCount = 1;

		VK_CHECK(vkCreateDevice(r->physicalDevice, &deviceCreateInfo, NULL, &r->device));
	}
	else
	{
		VkDeviceQueueCreateInfo queueGraphicsCreateInfo = {};
		queueGraphicsCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueGraphicsCreateInfo.pNext = nullptr;
		queueGraphicsCreateInfo.queueFamilyIndex = r->queueFamilyGraphics;
		queueGraphicsCreateInfo.queueCount = 1;
		queueGraphicsCreateInfo.pQueuePriorities = &queuePriority;

		VkDeviceQueueCreateInfo queuePresentCreateInfo = {};
		queuePresentCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queuePresentCreateInfo.pNext = nullptr;
		queuePresentCreateInfo.queueFamilyIndex = r->queueFamilyPresent;
		queuePresentCreateInfo.queueCount = 1;
		queuePresentCreateInfo.pQueuePriorities = &queuePriority;

		std::array<VkDeviceQueueCreateInfo, 2> queueCreateInfos = {queueGraphicsCreateInfo, queuePresentCreateInfo };

		deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		VK_CHECK(vkCreateDevice(r->physicalDevice, &deviceCreateInfo, NULL, &r->device));
	}

	vkGetDeviceQueue(r->device, r->queueFamilyGraphics, 0, &r->queueGraphics);
	vkGetDeviceQueue(r->device, r->queueFamilyPresent, 0, &r->queuePresent);
}

static void FindQueueFamilies(Render *r)
{
	uint32_t familyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(r->physicalDevice, &familyCount, nullptr);
	std::vector<VkQueueFamilyProperties> properties(familyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(r->physicalDevice, &familyCount, properties.data());

	r->queueFamilyGraphics = std::numeric_limits<uint32_t>::max();
	r->queueFamilyPresent = std::numeric_limits<uint32_t>::max();

	for (uint32_t i = 0; i < familyCount; i++)
	{
		if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			VkBool32 present;
			VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(r->physicalDevice, i, r->surface, &present));

			if (present)
			{
				r->queueFamilyGraphics = i;
				r->queueFamilyPresent = i;
				break;
			}

			if (r->queueFamilyGraphics == std::numeric_limits<uint32_t>::max())
			{
				r->queueFamilyGraphics = i;
			}
		}
	}

	if (r->queueFamilyPresent == std::numeric_limits<uint32_t>::max())
	{
		for (uint32_t i = 0; i < familyCount; i++)
		{
			VkBool32 present;
			VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(r->physicalDevice, i, r->surface, &present));

			if (present)
			{
				r->queueFamilyPresent = i;
				break;
			}
		}
	}
}

static void ChooseSurfaceFormat(Render *r)
{
	uint32_t formatCount;
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(r->physicalDevice, r->surface, &formatCount, NULL));

	assert(formatCount > 0);

	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(r->physicalDevice, r->surface, &formatCount, formats.data()));
	/*
	if (format_count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		r->format = VK_FORMAT_B8G8R8A8_UNORM;
	else r->format = formats[0].format;
	*/
	r->format = VK_FORMAT_B8G8R8A8_SRGB;
}

static void ChoosePresentMode(Render *r)
{
	uint32_t presentModeCount;
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(r->physicalDevice, r->surface, &presentModeCount, NULL));
	assert(presentModeCount > 0);
	std::vector<VkPresentModeKHR> present_modes(presentModeCount);
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(r->physicalDevice, r->surface, &presentModeCount, present_modes.data()));

	for (uint32_t i = 0; i < presentModeCount; i++)
	{
		if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			r->presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			return;
		}
	}

//	r->presentMode = VK_PRESENT_MODE_FIFO_KHR;
	r->presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
}

static void CreateSwapChain(Render *r)
{
	VkSurfaceCapabilitiesKHR capabilities;
	VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(r->physicalDevice, r->surface, &capabilities));

	uint32_t minImageCount = capabilities.minImageCount + 1;

	if (capabilities.maxImageCount &&minImageCount > capabilities.maxImageCount)
	{
		minImageCount = capabilities.maxImageCount;
	}

	ChooseSurfaceFormat(r);
	ChoosePresentMode(r);

	VkSwapchainKHR oldSwapchain = r->swapchain;
	r->swapchain = nullptr;

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = nullptr;
	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = r->surface;
	swapchainCreateInfo.minImageCount = minImageCount;
	swapchainCreateInfo.imageFormat = r->format;
	swapchainCreateInfo.imageColorSpace = r->colorSpace;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.preTransform = capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = r->presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = oldSwapchain;

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		swapchainCreateInfo.imageExtent = capabilities.currentExtent;
	}
	else
	{
		int windowW, windowH;
		glfwGetWindowSize(r->window, &windowW, &windowH);

		uint32_t w = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, (uint32_t)windowW));
		uint32_t h = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, (uint32_t)windowH));
		swapchainCreateInfo.imageExtent.width = w;
		swapchainCreateInfo.imageExtent.height = h;
	}

	r->swapchainExtent = swapchainCreateInfo.imageExtent;

	if (r->queueFamilyGraphics != r->queueFamilyPresent)
	{
		uint32_t families[] = { r->queueFamilyGraphics, r->queueFamilyPresent };
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = families;
	}
	else
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	VK_CHECK(vkCreateSwapchainKHR(r->device, &swapchainCreateInfo, NULL, &r->swapchain));

	if (oldSwapchain)
	{
		vkDestroySwapchainKHR(r->device, oldSwapchain, nullptr);
	}

	VK_CHECK(vkGetSwapchainImagesKHR(r->device, r->swapchain, &r->imageCount, NULL));

	r->images.resize(r->imageCount);
	VK_CHECK(vkGetSwapchainImagesKHR(r->device, r->swapchain, &r->imageCount, r->images.data()));
}

static void CreateImageViews(Render *r)
{
	r->imageViews.resize(r->imageCount);

	for (uint32_t i = 0; i < r->imageCount; i++)
	{
		VkImageViewCreateInfo viewCreateInfo = {};
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewCreateInfo.image = r->images[i];
		viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewCreateInfo.format = r->format;
		viewCreateInfo.components = {};
		viewCreateInfo.subresourceRange = {};
		viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewCreateInfo.subresourceRange.baseMipLevel = 0;
		viewCreateInfo.subresourceRange.levelCount = 1;
		viewCreateInfo.subresourceRange.baseArrayLayer = 0;
		viewCreateInfo.subresourceRange.layerCount = 1;

		VK_CHECK(vkCreateImageView(r->device, &viewCreateInfo, NULL, &r->imageViews[i]));
	}
}

static void CreateRenderPass(Render *r)
{
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = r->format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	std::array<VkAttachmentDescription, 1> attachments = {colorAttachment};

	VkAttachmentReference colorAttachmentReference = {};
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentReference;

	VkSubpassDependency subpassDependency = {};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = attachments.size();
	renderPassCreateInfo.pAttachments = attachments.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

	VK_CHECK(vkCreateRenderPass(r->device, &renderPassCreateInfo, NULL, &r->renderPass));
}

static void CreateFramebuffers(Render *r)
{
	r->framebuffers.resize(r->imageCount);

	for (uint32_t i = 0; i < r->imageCount; i++)
	{
		VkImageView attachments[] = {
			r->imageViews[i],
		};

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = r->renderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.width = r->swapchainExtent.width;
		framebufferCreateInfo.height = r->swapchainExtent.height;
		framebufferCreateInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(r->device, &framebufferCreateInfo, NULL, &r->framebuffers[i]));
	}
}

static void CreateCommandPool(Render *r)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = r->queueFamilyGraphics;

	VK_CHECK(vkCreateCommandPool(r->device, &commandPoolCreateInfo, NULL, &r->commandPool));
}

static void BeginText(Render *r)
{
	r->glyphInstanceCount = 0;
	uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

	VK_CHECK(vkMapMemory(r->device, r->instanceStagingBufferMemory, 0, size, 0, reinterpret_cast<void **>(&r->glyphInstances)));
}

static void EndText(Render *r)
{
	VkCommandBuffer commandBuffer = r->commandBuffer;
	uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

	vkUnmapMemory(r->device, r->instanceStagingBufferMemory);

	VkBufferMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	barrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.buffer = r->instanceBuffer;
	barrier.offset = 0;
	barrier.size = size;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
		0, 0, nullptr, 1, &barrier, 0, nullptr);

	VkBufferCopy copy = {};
	copy.srcOffset = 0;
	copy.dstOffset = 0;
	copy.size = size;

	vkCmdCopyBuffer(commandBuffer, r->instanceStagingBuffer, r->instanceBuffer, 1, &copy);

	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
		0, 0, nullptr, 1, &barrier, 0, nullptr);
}

static void AppendText(Render *r, const float &x, const float &y, const float &scale, const std::wstring &text, const Colour &colour)
{
	float localX = x;

	for (const auto &c : text)
	{
		if (r->glyphInstanceCount >= MAX_VISIBLE_GLYPHS)
		{
			break;
		}

		uint32_t glyphIndex = r->charmap[c];
		HostGlyphInfo *gi = &r->glyphInfos[glyphIndex];
		GlyphInstance *inst = &r->glyphInstances[r->glyphInstanceCount];

		inst->rect.minX = (localX + gi->bbox.minX * scale) / (r->swapchainExtent.width / 2.0f) - 1.0f;
		inst->rect.minY = (y - gi->bbox.minY * scale) / (r->swapchainExtent.height / 2.0f) - 1.0f;
		inst->rect.maxX = (localX + gi->bbox.maxX * scale) / (r->swapchainExtent.width / 2.0f) - 1.0f;
		inst->rect.maxY = (y - gi->bbox.maxY * scale) / (r->swapchainExtent.height / 2.0f) - 1.0f;

		if (inst->rect.minX <= 1 && inst->rect.maxX >= -1 &&
			inst->rect.maxY <= 1 && inst->rect.minY >= -1)
		{
			inst->glyphIndex = glyphIndex;
			inst->sharpness = scale;
			inst->colour = colour;

			r->glyphInstanceCount++;
		}

		localX += gi->advance * scale;
	}
}

static void AppendText(Render *r, const float &x, const float &y, const float &scale, const std::string &text, const Colour &colour)
{
	std::wstring wsTmp(text.begin(), text.end());
	AppendText(r, x, y, scale, wsTmp, colour);
}

static void RecordCommandBuffer(Render *r)
{
	VkCommandBuffer commandBuffer = r->commandBuffer;

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &begin_info));

	BeginText(r);

	static std::vector<std::wstring> lines = {
		L"Hello world, Привет мир, schön! 0123456789 #$%^*@&( []{} «»½¼±¶§",
		L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus sit amet scelerisque augue, sit amet commodo neque. Vestibulum",
		L"eu eros a justo molestie bibendum quis in urna. Integer quis tristique magna. Morbi in ultricies lorem. Donec lacinia nisi et",
		L"arcu scelerisque, eget viverra ante dapibus. Proin enim neque, vehicula id congue quis, consequat sit amet tortor.Aenean ac",
		L"lorem sit amet magna rhoncus rhoncus ac ac neque. Cras sed rutrum sem. Donec placerat ultricies ex, a gravida lorem commodo ut.",
		L"Mauris faucibus aliquet ligula, vitae condimentum dui semper et. Aenean pellentesque ac ligula a varius. Suspendisse congue",
		L"lorem lorem, ac consectetur ipsum condimentum id.",
		L"",
		L"Vestibulum quis erat sem. Fusce efficitur libero et leo sagittis, ac volutpat felis ullamcorper. Curabitur fringilla eros eget ex",
		L"lobortis, at posuere sem consectetur. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis",
		L"egestas. Vivamus eu enim leo. Morbi ultricies lorem et pellentesque vestibulum. Proin eu ultricies sem. Quisque laoreet, ligula",
		L"non molestie congue, odio nunc tempus arcu, vel aliquet leo turpis non enim. Sed risus dui, condimentum et eros a, molestie",
		L"imperdiet nisl. Vivamus quis ante venenatis, cursus magna ut, tincidunt elit. Aenean nisl risus, porttitor et viverra quis,",
		L"tempus vitae nisl.",
		L"",
		L"Suspendisse ut scelerisque tellus. In ac quam sem.Curabitur suscipit massa nisl. Ut et metus sed lacus dapibus molestie. Nullam",
		L"porttitor sit amet magna quis dapibus. Nulla tincidunt, arcu sit amet hendrerit consequat, felis leo blandit libero, eu posuere",
		L"nisl quam interdum nulla. Quisque nec efficitur libero. Quisque quis orci vitae metus feugiat aliquam eu et nulla. Etiam aliquet",
		L"ante vitae lacus aliquam, et gravida elit mollis. Proin molestie, justo tempus rhoncus aliquam, tellus erat venenatis erat,",
		L"porttitor dapibus nunc purus id enim. Integer a nunc ut velit porta maximus. Nullam rutrum nisi in sagittis pharetra. Proin id",
		L"pharetra augue, sed vulputate lorem. Aenean dapibus, turpis nec ullamcorper pharetra, ex augue congue nibh, condimentum",
		L"vestibulum arcu urna quis ex.",
		L"",
		L"Vestibulum non dignissim nibh, quis vestibulum odio. Ut sed viverra ante, fringilla convallis tellus. Donec in est rutrum,",
		L"imperdiet dolor a, vestibulum magna. In nec justo tellus. Ut non erat eu leo ornare imperdiet in sit amet lorem. Nullam quis",
		L"nisl diam. Aliquam laoreet dui et ligula posuere cursus.",
		L"",
		L"Donec vestibulum ante eget arcu dapibus lobortis.Curabitur condimentum tellus felis, id luctus mi ultrices quis. Aenean nulla",
		L"justo, venenatis vel risus et, suscipit faucibus nulla. Pellentesque habitant morbi tristique senectus et netus et malesuada",
		L"fames ac turpis egestas. Sed lacinia metus eleifend lacinia blandit.Morbi est nibh, dapibus nec arcu quis, volutpat lacinia",
		L"dolor. Vestibulum quis viverra erat.Maecenas ultricies odio neque, et eleifend arcu auctor in. Suspendisse placerat massa nisl,",
		L"non condimentum ligula sodales at.Phasellus eros urna, elementum in ultricies quis, vulputate id magna. Donec efficitur rutrum",
		L"urna sed tempus. Vestibulum eu augue dolor. Vestibulum vehicula suscipit purus, sit amet ultricies ligula malesuada sit amet.",
		L"Duis consectetur elit euismod arcu aliquet vehicula. Pellentesque lobortis dui et nisl vehicula, in placerat quam dapibus.Fusce",
		L"auctor arcu a purus bibendum, eget blandit nisi lobortis."
	};

	for (uint32_t i = 0; i < lines.size(); i++)
	{
		AppendText(r, r->canvasScale * (10.0f - r->canvasOffset.m_x),
			r->canvasScale * (30.0f - r->canvasOffset.m_y + i * 30.0f),
		    0.02f * r->canvasScale, lines[i], Colour::Black);
	}

	AppendText(r, 5.0f, 25.0f, 0.02f, "Frame Time: " + String::To(1000.0f / r->fps) + "ms", Colour::Red);
	AppendText(r, 5.0f, 55.0f, 0.02f, "Fps: " + String::To(r->fps), Colour::Green);

	EndText(r);

	VkClearValue clearValue = {1.0f, 1.0f, 1.0f, 1.0f};
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = r->renderPass;
	renderPassBeginInfo.framebuffer = r->framebuffers[r->imageIndex];
	renderPassBeginInfo.renderArea = {};
	renderPassBeginInfo.renderArea.offset = {0, 0},
	renderPassBeginInfo.renderArea.extent = r->swapchainExtent;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &r->instanceBuffer, offsets);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,r->pipelineLayout, 0, 1, 
		&r->descriptorSet, 0, nullptr);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, r->pipeline);
	vkCmdDraw(commandBuffer, 4, r->glyphInstanceCount, 0, 0);

	vkCmdEndRenderPass(commandBuffer);
	VK_CHECK(vkEndCommandBuffer(commandBuffer));
}

static void CreateCommandBuffer(Render *r)
{
	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = r->commandPool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = 1;

	VK_CHECK(vkAllocateCommandBuffers(r->device, &allocateInfo, &r->commandBuffer));

	RecordCommandBuffer(r);
}

static void CreateCommandBufferFence(Render *r)
{
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VK_CHECK(vkCreateFence(r->device, &fenceCreateInfo, NULL, &r->commandBufferFence));
}

static void CreateSemaphores(Render *r)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VK_CHECK(vkCreateSemaphore(r->device, &semaphoreCreateInfo, NULL, &r->imageAvailableSemaphore));
	VK_CHECK(vkCreateSemaphore(r->device, &semaphoreCreateInfo, NULL, &r->renderFinishedSemaphore));
}

static VkShaderModule LoadShaderModule(const VkDevice &device, const char *path)
{
	FILE *f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);

	auto code = std::vector<uint32_t>(size / sizeof(uint32_t));
	fread(code.data(), size, 1, f);
	fclose(f);

	VkShaderModuleCreateInfo moduleCreateInfo = {};
	moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleCreateInfo.codeSize = static_cast<std::size_t>(size);
	moduleCreateInfo.pCode = code.data();

	VkShaderModule ret;
	VK_CHECK(vkCreateShaderModule(device, &moduleCreateInfo, NULL, &ret));
	return ret;
}

static void CreateLayout(Render *r)
{
	VkDescriptorSetLayoutBinding bufferGlyphBinding = {};
	bufferGlyphBinding.binding = 0;
	bufferGlyphBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	bufferGlyphBinding.descriptorCount = 1;
	bufferGlyphBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding bufferCellBinding = {};
	bufferCellBinding.binding = 1;
	bufferCellBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	bufferCellBinding.descriptorCount = 1;
	bufferCellBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding bufferPointBinding = {};
	bufferPointBinding.binding = 2;
	bufferPointBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	bufferPointBinding.descriptorCount = 1;
	bufferPointBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 3> bindings = {bufferGlyphBinding, bufferCellBinding, bufferPointBinding};

	VkDescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo = {};
	descriptorLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorLayoutCreateInfo.bindingCount = bindings.size();
	descriptorLayoutCreateInfo.pBindings = bindings.data();

	VK_CHECK(vkCreateDescriptorSetLayout(r->device, &descriptorLayoutCreateInfo, NULL, &r->setLayout));

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &r->setLayout;

	VK_CHECK(vkCreatePipelineLayout(r->device, &pipelineLayoutCreateInfo, NULL, &r->pipelineLayout));
}

static uint32_t FindMemoryType(Render *r, const uint32_t &type_bits, const VkMemoryPropertyFlags &flags)
{
	for (uint32_t i = 0; i < r->memoryProperties.memoryTypeCount; i++)
	{
		if (type_bits & 1 << i)
		{
			VkMemoryPropertyFlags f = r->memoryProperties.memoryTypes[i].propertyFlags;

			if ((f & flags) == flags)
			{
				return i;
			}
		}
	}

	return std::numeric_limits<uint32_t>::max();
}

static VkDeviceMemory AllocRequiredMemory(Render *r, VkMemoryRequirements *req, const VkMemoryPropertyFlags &flags)
{
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = req->size;
	memoryAllocateInfo.memoryTypeIndex = FindMemoryType(r, req->memoryTypeBits, flags);

	VkDeviceMemory memory;
	VK_CHECK(vkAllocateMemory(r->device, &memoryAllocateInfo, NULL, &memory));
	return memory;
}

static void CreateBufferWithMemory(Render *r, VkBufferCreateInfo *ci, const VkMemoryPropertyFlags &flags,
	VkDeviceMemory *memory, VkBuffer *buffer)
{
	VK_CHECK(vkCreateBuffer(r->device, ci, NULL, buffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(r->device, *buffer, &requirements);

	*memory = AllocRequiredMemory(r, &requirements, flags);
	VK_CHECK(vkBindBufferMemory(r->device, *buffer, *memory, 0));
}

VkCommandBuffer BeginOneTimeCmdbuf(Render *r)
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = r->commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	VK_CHECK(vkAllocateCommandBuffers(r->device, &commandBufferAllocateInfo, &commandBuffer));

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));
	return commandBuffer;
}

void EndOneTimeCmdbuf(Render *r, const VkCommandBuffer &commandBuffer)
{
	VK_CHECK(vkEndCommandBuffer(commandBuffer));

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VK_CHECK(vkQueueSubmit(r->queueGraphics, 1, &submitInfo, VK_NULL_HANDLE));
	VK_CHECK(vkQueueWaitIdle(r->queueGraphics));
	vkFreeCommandBuffers(r->device, r->commandPool, 1, &commandBuffer);
}

static void CopyBuffer(Render *r, const VkBuffer &srcBuffer, const VkBuffer &dstBuffer, const VkDeviceSize &size)
{
	VkCommandBuffer commandBuffer = BeginOneTimeCmdbuf(r);
	VkBufferCopy copy = {0, 0, size};

	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copy);
	EndOneTimeCmdbuf(r, commandBuffer);
}

static void StageBuffer(Render *r, const VkBuffer &buffer, const void *data, const std::size_t &size)
{
	VkBufferCreateInfo stagingCreateInfo = {};
	stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingCreateInfo.size = size;
	stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBufferWithMemory(r, &stagingCreateInfo, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		&stagingBufferMemory, &stagingBuffer);

	void *stagingBufferPtr;
	VK_CHECK(vkMapMemory(r->device, stagingBufferMemory, 0, stagingCreateInfo.size, 0, &stagingBufferPtr));

	memcpy(stagingBufferPtr, data, size);

	vkUnmapMemory(r->device, stagingBufferMemory);

	CopyBuffer(r, stagingBuffer, buffer, size);

	vkDestroyBuffer(r->device, stagingBuffer, nullptr);
	vkFreeMemory(r->device, stagingBufferMemory, nullptr);
}

static void CreateStorageBuffer(const std::string &filename, Render *r)
{
	LoadFont(filename, r);

	VkBufferCreateInfo storageCreateInfo = {};
	storageCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	storageCreateInfo.size = r->glyphDataSize;
	storageCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	storageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateBufferWithMemory(r, &storageCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&r->storageBufferMemory, &r->storageBuffer);

	StageBuffer(r, r->storageBuffer, r->glyphData.get(), storageCreateInfo.size);

	r->glyphData = nullptr;
}

static void CreateInstanceBuffer(Render *r)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateBufferWithMemory(r, &bufferCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&r->instanceBufferMemory, &r->instanceBuffer);

	VkBufferCreateInfo stagingCreateInfo = {};
	stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingCreateInfo.size = bufferCreateInfo.size;
	stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateBufferWithMemory(r, &stagingCreateInfo, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		&r->instanceStagingBufferMemory, &r->instanceStagingBuffer);
}

static void CreateDescriptorPool(Render *r)
{
	VkDescriptorPoolSize poolSizes[] = {
		{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3},
	};

	VkDescriptorPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.maxSets = 1;
	poolCreateInfo.poolSizeCount = 1;
	poolCreateInfo.pPoolSizes = poolSizes;

	VK_CHECK(vkCreateDescriptorPool(r->device, &poolCreateInfo, NULL, &r->descriptorPool));
}

static void CreateDescriptorSet(Render *r)
{
	VkDescriptorSetAllocateInfo setAllocateInfo = {};
	setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocateInfo.descriptorPool = r->descriptorPool;
	setAllocateInfo.descriptorSetCount = 1;
	setAllocateInfo.pSetLayouts = &r->setLayout;

	VK_CHECK(vkAllocateDescriptorSets(r->device, &setAllocateInfo, &r->descriptorSet));

	VkDescriptorBufferInfo glyphInfo = {};
	glyphInfo.buffer = r->storageBuffer;
	glyphInfo.offset = r->glyphInfoOffset;
	glyphInfo.range = r->glyphInfoSize;

	VkDescriptorBufferInfo cellsInfo = {};
	cellsInfo.buffer = r->storageBuffer;
	cellsInfo.offset = r->glyphCellsOffset;
	cellsInfo.range = r->glyphCellsSize;

	VkDescriptorBufferInfo pointsInfo = {};
	pointsInfo.buffer = r->storageBuffer;
	pointsInfo.offset = r->glyphPointsOffset;
	pointsInfo.range = r->glyphPointsSize;

	std::array<VkWriteDescriptorSet, 3> writes = {};

	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].dstSet = r->descriptorSet;
	writes[0].dstBinding = 0;
	writes[0].dstArrayElement = 0;
	writes[0].descriptorCount = 1;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[0].pBufferInfo = &glyphInfo;

	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].dstSet = r->descriptorSet;
	writes[1].dstBinding = 1;
	writes[1].dstArrayElement = 0;
	writes[1].descriptorCount = 1;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[1].pBufferInfo = &cellsInfo;

	writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[2].dstSet = r->descriptorSet;
	writes[2].dstBinding = 2;
	writes[2].dstArrayElement = 0;
	writes[2].descriptorCount = 1;
	writes[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[2].pBufferInfo = &pointsInfo;

	vkUpdateDescriptorSets(r->device, writes.size(), writes.data(), 0, nullptr);
}

static void CreatePipeline(Render *r)
{
	VkShaderModule vsFont = LoadShaderModule(r->device, "shaders/font.vert.spv");
	VkShaderModule fsFont = LoadShaderModule(r->device, "shaders/font.frag.spv");

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vsFont;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fsFont;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	VkVertexInputBindingDescription vertexInputBinding =
		{0, sizeof(GlyphInstance), VK_VERTEX_INPUT_RATE_INSTANCE};

	VkVertexInputAttributeDescription vertexInputAttributes[] = {
		{0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(GlyphInstance, rect)},
		{1, 0, VK_FORMAT_R32_UINT, offsetof(GlyphInstance, glyphIndex)},
		{2, 0, VK_FORMAT_R32_SFLOAT, offsetof(GlyphInstance, sharpness)},
		{3, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(GlyphInstance, colour)}
	};

	VkPipelineVertexInputStateCreateInfo vertexInputState = {};
	vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputState.vertexBindingDescriptionCount = 1;
	vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
	vertexInputState.vertexAttributeDescriptionCount = 4;
	vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	inputAssemblyState.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(r->swapchainExtent.width);
	viewport.height = static_cast<float>(r->swapchainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = r->swapchainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizerState = {};
	rasterizerState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerState.depthClampEnable = VK_FALSE;
	rasterizerState.rasterizerDiscardEnable = VK_FALSE;
	rasterizerState.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizerState.cullMode = VK_CULL_MODE_NONE;
	rasterizerState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizerState.depthBiasEnable = VK_FALSE;
	rasterizerState.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.sampleShadingEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState blendAttachmentState = {};
	blendAttachmentState.blendEnable = VK_TRUE;
	blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
	blendAttachmentState.alphaBlendOp = VK_BLEND_OP_MAX;
	blendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo blendSate = {};
	blendSate.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendSate.logicOpEnable = VK_FALSE;
	blendSate.logicOp = VK_LOGIC_OP_COPY;
	blendSate.attachmentCount = 1;
	blendSate.pAttachments = &blendAttachmentState;

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputState;
	pipelineInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizerState;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &blendSate;
	pipelineInfo.layout = r->pipelineLayout;
	pipelineInfo.renderPass = r->renderPass;
	pipelineInfo.subpass = 0;

	VK_CHECK(vkCreateGraphicsPipelines(r->device, VK_NULL_HANDLE, 1,
		&pipelineInfo, nullptr, &r->pipeline));

	vkDestroyShaderModule(r->device, vsFont, nullptr);
	vkDestroyShaderModule(r->device, fsFont, nullptr);
}

static void CreateSwapChainObjects(Render *r)
{
	CreateImageViews(r);
	CreateRenderPass(r);
	CreatePipeline(r);
	CreateFramebuffers(r);

	CreateCommandBuffer(r);
	CreateCommandBufferFence(r);
}

static void CreateVulkanObjects(const std::string &filename, Render *r)
{
	CreateInstance(r);
	CreateSurface(r);
	PickPhysicalDevice(r);
	FindQueueFamilies(r);
	CreateDevice(r);
	CreateCommandPool(r);
	CreateLayout(r);
	CreateStorageBuffer(filename, r);
	CreateInstanceBuffer(r);
	CreateDescriptorPool(r);
	CreateDescriptorSet(r);

	CreateSwapChain(r);
	CreateSwapChainObjects(r);
	CreateSemaphores(r);
}

static void DestroySwapChainObjects(Render *r)
{
	VK_DESTROY(vkDestroyPipeline, r->device, r->pipeline);

	vkDestroyFence(r->device, r->commandBufferFence, nullptr);

	vkFreeCommandBuffers(r->device, r->commandPool, 1, &r->commandBuffer);

	for (uint32_t i = 0; i < r->imageCount; i++)
	{
		vkDestroyFramebuffer(r->device, r->framebuffers[i], nullptr);
	}

	VK_DESTROY(vkDestroyRenderPass, r->device, r->renderPass);

	for (uint32_t i = 0; i < r->imageCount; i++)
	{
		vkDestroyImageView(r->device, r->imageViews[i], nullptr);
	}
}

static void DestroyVulkanObjects(Render *r)
{
//	vkDeviceWaitIdle(r->device);

	DestroySwapChainObjects(r);
	
	VK_DESTROY(vkDestroyBuffer, r->device, r->instanceStagingBuffer);
	VK_DESTROY(vkFreeMemory, r->device, r->instanceStagingBufferMemory);
	VK_DESTROY(vkDestroyBuffer, r->device, r->instanceBuffer);
	VK_DESTROY(vkFreeMemory, r->device, r->instanceBufferMemory);
	VK_DESTROY(vkDestroyBuffer, r->device, r->storageBuffer);
	VK_DESTROY(vkFreeMemory, r->device, r->storageBufferMemory);
	VK_DESTROY(vkDestroyDescriptorPool, r->device, r->descriptorPool);
	VK_DESTROY(vkDestroyDescriptorSetLayout, r->device, r->setLayout);
	VK_DESTROY(vkDestroyPipelineLayout, r->device, r->pipelineLayout);
	VK_DESTROY(vkDestroySwapchainKHR, r->device, r->swapchain);
	VK_DESTROY(vkDestroySemaphore, r->device, r->imageAvailableSemaphore);
	VK_DESTROY(vkDestroySemaphore, r->device, r->renderFinishedSemaphore);
	VK_DESTROY(vkDestroyCommandPool, r->device, r->commandPool);

	vkDestroyDevice(r->device, nullptr);
	r->device = nullptr;

	vkDestroySurfaceKHR(r->instance, r->surface, nullptr);
	r->surface = nullptr;

	vkDestroyInstance(r->instance, nullptr);
	r->instance = nullptr;
}

static void RecreateSwapChain(Render *r)
{
	vkDeviceWaitIdle(r->device);

	DestroySwapChainObjects(r);
	CreateSwapChain(r);
	CreateSwapChainObjects(r);
}

static void Update(Render *r)
{
	r->frame++;
	r->fpsTemp++;

	auto curr = static_cast<float>(glfwGetTime());

	if (floorf(curr) > floorf(r->time))
	{
		r->fps = r->fpsTemp;
		r->fpsTemp = 0;
	}

	r->deltaTime = (float)(curr - r->time);
	r->time = curr;

	double xpos, ypos;
	glfwGetCursorPos(r->window, &xpos, &ypos);

	r->mousePos.m_x = static_cast<float>(xpos);
	r->mousePos.m_y = static_cast<float>(ypos);

	Vector2 mouseDelta = r->oldMousePos - r->mousePos;

	if (glfwGetMouseButton(r->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		mouseDelta *= 1.0f / r->canvasScale;
		r->canvasOffset += mouseDelta;
	}

	r->oldMousePos = r->mousePos;

	Vector2 swapchainExtent = {
		static_cast<float>(r->swapchainExtent.width),
		static_cast<float>(r->swapchainExtent.height)
	};

	Vector2 oldSize = swapchainExtent * (1.0f / r->canvasScale);

	if (r->canvasScale != r->targetCanvasScale)
	{
		r->canvasScale = Maths::Lerp(r->canvasScale, r->targetCanvasScale, r->deltaTime * 30.0f);

		Vector2 newSize = swapchainExtent * (1.0f / r->canvasScale);
		Vector2 tmp = oldSize - newSize;

		tmp.m_x *= r->mousePos.m_x / swapchainExtent.m_x;
		tmp.m_y *= r->mousePos.m_y / swapchainExtent.m_y;

		r->canvasOffset += tmp;
	}
}

static void RenderFrame(Render *r)
{
	VkResult res = vkAcquireNextImageKHR(r->device, r->swapchain, std::numeric_limits<uint32_t>::max(),
		r->imageAvailableSemaphore, nullptr, &r->imageIndex);

	if (res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain(r);
		return;
	}
	if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR)
	{
		return;
	}

	VkFence currentFence = r->commandBufferFence;
	vkWaitForFences(r->device, 1, &currentFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	vkResetFences(r->device, 1, &currentFence);

	RecordCommandBuffer(r);

	VkSemaphore waitSemaphores[] = {r->imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkSemaphore signalSemaphores[] = {r->renderFinishedSemaphore};

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &r->commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	res = vkQueueSubmit(r->queueGraphics, 1, &submitInfo, currentFence);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &r->swapchain;
	presentInfo.pImageIndices = &r->imageIndex;

	res = vkQueuePresentKHR(r->queuePresent, &presentInfo);

	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
	{
		RecreateSwapChain(r);
	//	return true;
	}
	else
	{
	//	return res == VK_SUCCESS;
	}
}

static void OnWindowResized(GLFWwindow *window, int width, int height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	RecreateSwapChain((Render *)(glfwGetWindowUserPointer(window)));
}

void WindowRefreshCallback(GLFWwindow *window)
{
	Render *r = (Render *)(glfwGetWindowUserPointer(window));

	Update(r);
	RenderFrame(r);
}

void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	Render *render = (Render *)(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
	}
}

void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	Render *r = (Render *)(glfwGetWindowUserPointer(window));

	r->targetCanvasScale *= powf(1.3f, (float)yoffset);
}

int main(int argc, const char **args)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

	Render render = {};
	render.canvasScale = 3.0f;
	render.targetCanvasScale = render.canvasScale;
	render.window = window;

	glfwSetWindowUserPointer(window, &render);
	glfwSetWindowSizeCallback(window, OnWindowResized);
	glfwSetWindowRefreshCallback(window, WindowRefreshCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	std::string filename = "Resources/Engine/Fonts/Alice-Regular.ttf";
//	std::string filename = "Resources/Engine/Fonts/marediv.ttf";
//	std::string filename = "Resources/Engine/Fonts/Lobster-Regular.ttf";
//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-Bold.ttf";
//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-BoldItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-Italic.ttf";
//	std::string filename = "Resources/Engine/Fonts/LobsterTwo-Regular.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-Bold.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-BoldItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-ExtraBold.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-ExtraBoldItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-Italic.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-Light.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-LightItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-Regular.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-SemiBold.ttf";
//	std::string filename = "Resources/Engine/Fonts/OpenSans-SemiBoldItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-Black.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-BlackItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-Bold.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-BoldItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-Italic.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-Light.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-LightItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-Medium.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-MediumItalic.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-Regular.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-Thin.ttf";
//	std::string filename = "Resources/Engine/Fonts/Roboto-ThinItalic.ttf";
	CreateVulkanObjects(filename, &render);

	while (!glfwWindowShouldClose(window))
	{
		Update(&render);
		RenderFrame(&render);
		glfwPollEvents();
	}

	DestroyVulkanObjects(&render);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#endif
