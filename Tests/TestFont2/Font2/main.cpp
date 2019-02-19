#if 1
#include <memory>
#include <array>
#include <cassert>
#include <vector>
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
#define NUMBER_OF_GLYPHS 96

struct CellInfo
{
	uint32_t point_offset;
	uint32_t cell_offset;
	uint32_t cell_count_x;
	uint32_t cell_count_y;
};

struct GlyphInstance
{
	Rect rect;
	uint32_t glyph_index;
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
	//Rect cbox;
	CellInfo cell_info;
};

struct Render
{
	uint32_t frame;
	uint32_t fps_temp;
	uint32_t fps;
	float time;
	float delta_time;

	Vector2 mouse_pos;
	Vector2 old_mouse_pos;
	Vector2 canvas_offset;
	float canvas_scale;
	float target_canvas_scale;

	Outline outlines[NUMBER_OF_GLYPHS];
	HostGlyphInfo glyph_infos[NUMBER_OF_GLYPHS];

	void *glyph_data;
	uint32_t glyph_data_size;
	uint32_t glyph_info_size;
	uint32_t glyph_cells_size;
	uint32_t glyph_points_size;
	uint32_t glyph_info_offset;
	uint32_t glyph_cells_offset;
	uint32_t glyph_points_offset;

	GlyphInstance *glyph_instances;
	uint32_t glyph_instance_count;

	GLFWwindow *window;
	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkPhysicalDeviceMemoryProperties memory_properties;
	VkPhysicalDeviceProperties device_properties;
	uint32_t graphics_queue_family;
	uint32_t present_queue_family;
	VkDevice device;
	VkQueue graphics_queue;
	VkQueue present_queue;
	VkSurfaceKHR surface;
	VkFormat format;
	VkColorSpaceKHR color_space;
	VkPresentModeKHR present_mode;
	VkSwapchainKHR swapchain;
	VkExtent2D swapchain_extent;
	uint32_t image_index;
	uint32_t image_count;
	std::vector<VkImage> images;
	std::vector<VkImageView> image_views;
	VkRenderPass render_pass;
	std::vector<VkFramebuffer> framebuffers;
	VkSemaphore image_available_semaphore;
	VkSemaphore render_finished_semaphore;

	VkDeviceMemory storage_buffer_memory;
	VkBuffer storage_buffer;
	VkDeviceMemory instance_buffer_memory;
	VkBuffer instance_buffer;
	VkDeviceMemory instance_staging_buffer_memory;
	VkBuffer instance_staging_buffer;
	VkCommandPool command_pool;
	VkCommandBuffer command_buffer;
	VkFence command_buffer_fence;
	VkDescriptorPool descriptor_pool;
	VkDescriptorSet descriptor_set;
	VkDescriptorSetLayout set_layout;
	VkPipelineLayout pipeline_layout;
	VkPipeline pipeline;
};

static uint32_t AlignUint32(uint32_t value, uint32_t alignment)
{
	return (value + alignment - 1) / alignment * alignment;
}

static void LoadFont(std::string filename, Render *r)
{
	FT_Library library;
	FT_CHECK(FT_Init_FreeType(&library));

	FT_Face face;
	FT_CHECK(FT_New_Face(library, filename.c_str(), 0, &face));

	FT_CHECK(FT_Set_Char_Size(face, 0, 1000 * 64, 96, 96));

	uint32_t totalPoints = 0;
	uint32_t totalCells = 0;

	for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
	{
		char c = ' ' + i;
		Outline *o = &r->outlines[i];
		HostGlyphInfo *hgi = &r->glyph_infos[i];

		FT_UInt glyph_index = FT_Get_Char_Index(face, c);
		FT_CHECK(FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_HINTING));

		OutlineConvert(&face->glyph->outline, o, c);

		hgi->bbox = o->bbox;
		hgi->advance = face->glyph->metrics.horiAdvance / 64.0f;

		totalPoints += o->num_of_points;
		totalCells += o->cell_count_x * o->cell_count_y;
	}

	r->glyph_info_size = sizeof(DeviceGlyphInfo) * NUMBER_OF_GLYPHS;
	r->glyph_cells_size = sizeof(uint32_t) * totalCells;
	r->glyph_points_size = sizeof(Vector2) * totalPoints;

	uint32_t alignment = r->device_properties.limits.minStorageBufferOffsetAlignment;
	r->glyph_info_offset = 0;
	r->glyph_cells_offset = AlignUint32(r->glyph_info_size, alignment);
	r->glyph_points_offset = AlignUint32(r->glyph_info_size + r->glyph_cells_size, alignment);
	r->glyph_data_size = r->glyph_points_offset + r->glyph_points_size;

	r->glyph_data = malloc(r->glyph_data_size);

	DeviceGlyphInfo *device_glyph_infos = (DeviceGlyphInfo*)((char*)r->glyph_data + r->glyph_info_offset);
	uint32_t *cells = (uint32_t*)((char*)r->glyph_data + r->glyph_cells_offset);
	Vector2 *points = (Vector2*)((char*)r->glyph_data + r->glyph_points_offset);

	uint32_t pointOffset = 0;
	uint32_t cellOffset = 0;

	for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
	{
		Outline *o = &r->outlines[i];
		DeviceGlyphInfo *dgi = &device_glyph_infos[i];

		dgi->cell_info.cell_count_x = o->cell_count_x;
		dgi->cell_info.cell_count_y = o->cell_count_y;
		dgi->cell_info.point_offset = pointOffset;
		dgi->cell_info.cell_offset = cellOffset;
		dgi->bbox = o->bbox;

		uint32_t cell_count = o->cell_count_x * o->cell_count_y;
		memcpy(cells + cellOffset, o->cells, sizeof(uint32_t) * cell_count);
		memcpy(points + pointOffset, o->points, sizeof(Vector2) * o->num_of_points);

	//	OutlineU16Points(o, &dgi->cbox, points + point_offset);

		pointOffset += o->num_of_points;
		cellOffset += cell_count;
	}

	assert(pointOffset == totalPoints);
	assert(cellOffset == totalCells);

	for (auto &outline : r->outlines)
	{
		OutlineDestroy(&outline);
	}

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

	r->physical_device = physicalDevices[0];

	vkGetPhysicalDeviceMemoryProperties(r->physical_device, &r->memory_properties);
	vkGetPhysicalDeviceProperties(r->physical_device, &r->device_properties);
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

	if (r->graphics_queue_family == r->present_queue_family)
	{
		VkDeviceQueueCreateInfo queue_info = {};
		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.pNext = nullptr;
		queue_info.queueFamilyIndex = r->graphics_queue_family;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = &queuePriority;

		deviceCreateInfo.pQueueCreateInfos = &queue_info;
		deviceCreateInfo.queueCreateInfoCount = 1;

		VK_CHECK(vkCreateDevice(r->physical_device, &deviceCreateInfo, NULL, &r->device));
	}
	else
	{
		VkDeviceQueueCreateInfo queueGraphicsCreateInfo = {};
		queueGraphicsCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueGraphicsCreateInfo.pNext = nullptr;
		queueGraphicsCreateInfo.queueFamilyIndex = r->graphics_queue_family;
		queueGraphicsCreateInfo.queueCount = 1;
		queueGraphicsCreateInfo.pQueuePriorities = &queuePriority;

		VkDeviceQueueCreateInfo queuePresentCreateInfo = {};
		queuePresentCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queuePresentCreateInfo.pNext = nullptr;
		queuePresentCreateInfo.queueFamilyIndex = r->present_queue_family;
		queuePresentCreateInfo.queueCount = 1;
		queuePresentCreateInfo.pQueuePriorities = &queuePriority;

		std::array<VkDeviceQueueCreateInfo, 2> queueCreateInfos = {queueGraphicsCreateInfo, queuePresentCreateInfo };

		deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

		VK_CHECK(vkCreateDevice(r->physical_device, &deviceCreateInfo, NULL, &r->device));
	}

	vkGetDeviceQueue(r->device, r->graphics_queue_family, 0, &r->graphics_queue);
	vkGetDeviceQueue(r->device, r->present_queue_family, 0, &r->present_queue);
}

static void FindQueueFamilies(Render *r)
{
	uint32_t familyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(r->physical_device, &familyCount, nullptr);
	std::vector<VkQueueFamilyProperties> properties(familyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(r->physical_device, &familyCount, properties.data());

	r->graphics_queue_family = std::numeric_limits<uint32_t>::max();
	r->present_queue_family = std::numeric_limits<uint32_t>::max();

	for (uint32_t i = 0; i < familyCount; i++)
	{
		if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			VkBool32 present;
			VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(r->physical_device, i, r->surface, &present));

			if (present)
			{
				r->graphics_queue_family = i;
				r->present_queue_family = i;
				break;
			}

			if (r->graphics_queue_family == std::numeric_limits<uint32_t>::max())
			{
				r->graphics_queue_family = i;
			}
		}
	}

	if (r->present_queue_family == std::numeric_limits<uint32_t>::max())
	{
		for (uint32_t i = 0; i < familyCount; i++)
		{
			VkBool32 present;
			VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(r->physical_device, i, r->surface, &present));

			if (present)
			{
				r->present_queue_family = i;
				break;
			}
		}
	}
}

static void ChooseSurfaceFormat(Render *r)
{
	uint32_t formatCount;
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(r->physical_device, r->surface, &formatCount, NULL));

	assert(formatCount > 0);

	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(r->physical_device, r->surface, &formatCount, formats.data()));
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
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(r->physical_device, r->surface, &presentModeCount, NULL));
	assert(presentModeCount > 0);
	std::vector<VkPresentModeKHR> present_modes(presentModeCount);
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(r->physical_device, r->surface, &presentModeCount, present_modes.data()));

	for (uint32_t i = 0; i < presentModeCount; i++)
	{
		if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			r->present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			return;
		}
	}

//	r->present_mode = VK_PRESENT_MODE_FIFO_KHR;
	r->present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
}

static void CreateSwapChain(Render *r)
{
	VkSurfaceCapabilitiesKHR capabilities;
	VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(r->physical_device, r->surface, &capabilities));

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
	swapchainCreateInfo.imageColorSpace = r->color_space;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.preTransform = capabilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = r->present_mode;
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

	r->swapchain_extent = swapchainCreateInfo.imageExtent;

	if (r->graphics_queue_family != r->present_queue_family)
	{
		uint32_t families[] = { r->graphics_queue_family, r->present_queue_family };
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

	VK_CHECK(vkGetSwapchainImagesKHR(r->device, r->swapchain, &r->image_count, NULL));

	r->images.resize(r->image_count);
	VK_CHECK(vkGetSwapchainImagesKHR(r->device, r->swapchain, &r->image_count, r->images.data()));
}

static void CreateImageViews(Render *r)
{
	r->image_views.resize(r->image_count);

	for (uint32_t i = 0; i < r->image_count; i++)
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

		VK_CHECK(vkCreateImageView(r->device, &viewCreateInfo, NULL, &r->image_views[i]));
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

	VK_CHECK(vkCreateRenderPass(r->device, &renderPassCreateInfo, NULL, &r->render_pass));
}

static void CreateFramebuffers(Render *r)
{
	r->framebuffers.resize(r->image_count);

	for (uint32_t i = 0; i < r->image_count; i++)
	{
		VkImageView attachments[] = {
			r->image_views[i],
		};

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = r->render_pass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.width = r->swapchain_extent.width;
		framebufferCreateInfo.height = r->swapchain_extent.height;
		framebufferCreateInfo.layers = 1;

		VK_CHECK(vkCreateFramebuffer(r->device, &framebufferCreateInfo, NULL, &r->framebuffers[i]));
	}
}

static void CreateCommandPool(Render *r)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = r->graphics_queue_family;

	VK_CHECK(vkCreateCommandPool(r->device, &commandPoolCreateInfo, NULL, &r->command_pool));
}

static void BeginText(Render *r)
{
	r->glyph_instance_count = 0;
	uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

	VK_CHECK(vkMapMemory(r->device, r->instance_staging_buffer_memory, 0, size, 0, reinterpret_cast<void **>(&r->glyph_instances)));
}

static void EndText(Render *r)
{
	VkCommandBuffer commandBuffer = r->command_buffer;
	uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

	vkUnmapMemory(r->device, r->instance_staging_buffer_memory);

	VkBufferMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	barrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.buffer = r->instance_buffer;
	barrier.offset = 0;
	barrier.size = size;

	vkCmdPipelineBarrier(
		commandBuffer,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		0,
		0, nullptr,
		1, &barrier,
		0, nullptr);

	VkBufferCopy copy = {};
	copy.srcOffset = 0;
	copy.dstOffset = 0;
	copy.size = size;

	vkCmdCopyBuffer(commandBuffer, r->instance_staging_buffer, r->instance_buffer, 1, &copy);

	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

	vkCmdPipelineBarrier(
		commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
		0,
		0, nullptr,
		1, &barrier,
		0, nullptr);
}

static void AppendText(Render *r, float x, float y, float scale, std::string text, Colour colour)
{
	for (char c : text)
	{
		if (r->glyph_instance_count >= MAX_VISIBLE_GLYPHS)
		{
			break;
		}

		uint32_t glyphIndex = c - 32;

		HostGlyphInfo *gi = &r->glyph_infos[glyphIndex];
		GlyphInstance *inst = &r->glyph_instances[r->glyph_instance_count];

		inst->rect.min_x = (x + gi->bbox.min_x * scale) / (r->swapchain_extent.width / 2.0f) - 1.0f;
		inst->rect.min_y = (y - gi->bbox.min_y * scale) / (r->swapchain_extent.height / 2.0f) - 1.0f;
		inst->rect.max_x = (x + gi->bbox.max_x * scale) / (r->swapchain_extent.width / 2.0f) - 1.0f;
		inst->rect.max_y = (y - gi->bbox.max_y * scale) / (r->swapchain_extent.height / 2.0f) - 1.0f;

		if (inst->rect.min_x <= 1 && inst->rect.max_x >= -1 &&
			inst->rect.max_y <= 1 && inst->rect.min_y >= -1)
		{
			inst->glyph_index = glyphIndex;
			inst->sharpness = scale;
			inst->colour = colour;

			r->glyph_instance_count++;
		}

		x += gi->advance * scale;
	}
}

static void RecordCommandBuffer(Render *r)
{
	VkCommandBuffer commandBuffer = r->command_buffer;

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VK_CHECK(vkBeginCommandBuffer(commandBuffer, &begin_info));

	BeginText(r);

	AppendText(r, 5.0f, 25.0f, 0.02f, "Frame Time: " + String::To(1000.0f / r->fps) + "ms", Colour::Red);
	AppendText(r, 5.0f, 55.0f, 0.02f, "Fps: " + String::To(r->fps), Colour::Green);

	static std::vector<std::string> lines = {
		"@&(3 Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus sit amet scelerisque augue, sit amet commodo neque. Vestibulum",
		"eu eros a justo molestie bibendum quis in urna. Integer quis tristique magna. Morbi in ultricies lorem. Donec lacinia nisi et",
		"arcu scelerisque, eget viverra ante dapibus. Proin enim neque, vehicula id congue quis, consequat sit amet tortor.Aenean ac",
		"lorem sit amet magna rhoncus rhoncus ac ac neque. Cras sed rutrum sem. Donec placerat ultricies ex, a gravida lorem commodo ut.",
		"Mauris faucibus aliquet ligula, vitae condimentum dui semper et. Aenean pellentesque ac ligula a varius. Suspendisse congue",
		"lorem lorem, ac consectetur ipsum condimentum id.",
		"",
		"Vestibulum quis erat sem. Fusce efficitur libero et leo sagittis, ac volutpat felis ullamcorper. Curabitur fringilla eros eget ex",
		"lobortis, at posuere sem consectetur. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis",
		"egestas. Vivamus eu enim leo. Morbi ultricies lorem et pellentesque vestibulum. Proin eu ultricies sem. Quisque laoreet, ligula",
		"non molestie congue, odio nunc tempus arcu, vel aliquet leo turpis non enim. Sed risus dui, condimentum et eros a, molestie",
		"imperdiet nisl. Vivamus quis ante venenatis, cursus magna ut, tincidunt elit. Aenean nisl risus, porttitor et viverra quis,",
		"tempus vitae nisl.",
		"",
		"Suspendisse ut scelerisque tellus. In ac quam sem.Curabitur suscipit massa nisl. Ut et metus sed lacus dapibus molestie. Nullam",
		"porttitor sit amet magna quis dapibus. Nulla tincidunt, arcu sit amet hendrerit consequat, felis leo blandit libero, eu posuere",
		"nisl quam interdum nulla. Quisque nec efficitur libero. Quisque quis orci vitae metus feugiat aliquam eu et nulla. Etiam aliquet",
		"ante vitae lacus aliquam, et gravida elit mollis. Proin molestie, justo tempus rhoncus aliquam, tellus erat venenatis erat,",
		"porttitor dapibus nunc purus id enim. Integer a nunc ut velit porta maximus. Nullam rutrum nisi in sagittis pharetra. Proin id",
		"pharetra augue, sed vulputate lorem. Aenean dapibus, turpis nec ullamcorper pharetra, ex augue congue nibh, condimentum",
		"vestibulum arcu urna quis ex.",
		"",
		"Vestibulum non dignissim nibh, quis vestibulum odio. Ut sed viverra ante, fringilla convallis tellus. Donec in est rutrum,",
		"imperdiet dolor a, vestibulum magna. In nec justo tellus. Ut non erat eu leo ornare imperdiet in sit amet lorem. Nullam quis",
		"nisl diam. Aliquam laoreet dui et ligula posuere cursus.",
		"",
		"Donec vestibulum ante eget arcu dapibus lobortis.Curabitur condimentum tellus felis, id luctus mi ultrices quis. Aenean nulla",
		"justo, venenatis vel risus et, suscipit faucibus nulla. Pellentesque habitant morbi tristique senectus et netus et malesuada",
		"fames ac turpis egestas. Sed lacinia metus eleifend lacinia blandit.Morbi est nibh, dapibus nec arcu quis, volutpat lacinia",
		"dolor. Vestibulum quis viverra erat.Maecenas ultricies odio neque, et eleifend arcu auctor in. Suspendisse placerat massa nisl,",
		"non condimentum ligula sodales at.Phasellus eros urna, elementum in ultricies quis, vulputate id magna. Donec efficitur rutrum",
		"urna sed tempus. Vestibulum eu augue dolor. Vestibulum vehicula suscipit purus, sit amet ultricies ligula malesuada sit amet.",
		"Duis consectetur elit euismod arcu aliquet vehicula. Pellentesque lobortis dui et nisl vehicula, in placerat quam dapibus.Fusce",
		"auctor arcu a purus bibendum, eget blandit nisi lobortis.",
	};

	for (uint32_t i = 0; i < lines.size(); i++)
	{
		AppendText(r,
		            r->canvas_scale * (10.0f - r->canvas_offset[0]),
		            r->canvas_scale * (30.0f - r->canvas_offset[1] + i * 30.0f),
		            0.02f * r->canvas_scale,
		            lines[i], Colour::Blue);
	}

	EndText(r);

	VkClearValue clear_value = {1.0f, 1.0f, 1.0f, 1.0f};
	VkRenderPassBeginInfo render_pass_bi = {};
	render_pass_bi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_bi.renderPass = r->render_pass;
	render_pass_bi.framebuffer = r->framebuffers[r->image_index];
	render_pass_bi.renderArea = {};
	render_pass_bi.renderArea.offset = {0, 0},
		render_pass_bi.renderArea.extent = r->swapchain_extent;
	render_pass_bi.clearValueCount = 1;
	render_pass_bi.pClearValues = &clear_value;

	vkCmdBeginRenderPass(commandBuffer, &render_pass_bi, VK_SUBPASS_CONTENTS_INLINE);

	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &r->instance_buffer, offsets);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
	                        r->pipeline_layout, 0, 1, &r->descriptor_set, 0, nullptr);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, r->pipeline);
	vkCmdDraw(commandBuffer, 4, r->glyph_instance_count, 0, 0);

	vkCmdEndRenderPass(commandBuffer);
	VK_CHECK(vkEndCommandBuffer(commandBuffer));
}

static void CreateCommandBuffer(Render *r)
{
	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.commandPool = r->command_pool;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandBufferCount = 1;

	VK_CHECK(vkAllocateCommandBuffers(r->device, &allocateInfo, &r->command_buffer));

	RecordCommandBuffer(r);
}

static void CreateCommandBufferFence(Render *r)
{
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VK_CHECK(vkCreateFence(r->device, &fenceCreateInfo, NULL, &r->command_buffer_fence));
}

static void CreateSemaphores(Render *r)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VK_CHECK(vkCreateSemaphore(r->device, &semaphoreCreateInfo, NULL, &r->image_available_semaphore));
	VK_CHECK(vkCreateSemaphore(r->device, &semaphoreCreateInfo, NULL, &r->render_finished_semaphore));
}

static VkShaderModule LoadShaderModule(VkDevice device, const char *path)
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
	moduleCreateInfo.codeSize = size_t(size);
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

	VK_CHECK(vkCreateDescriptorSetLayout(r->device, &descriptorLayoutCreateInfo, NULL, &r->set_layout));

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &r->set_layout;

	VK_CHECK(vkCreatePipelineLayout(r->device, &pipelineLayoutCreateInfo, NULL, &r->pipeline_layout));
}

static uint32_t FindMemoryType(Render *r, uint32_t type_bits, VkMemoryPropertyFlags flags)
{
	for (uint32_t i = 0; i < r->memory_properties.memoryTypeCount; i++)
	{
		if (type_bits & 1 << i)
		{
			VkMemoryPropertyFlags f = r->memory_properties.memoryTypes[i].propertyFlags;

			if ((f & flags) == flags)
			{
				return i;
			}
		}
	}

	return std::numeric_limits<uint32_t>::max();
}

static VkDeviceMemory AllocRequiredMemory(Render *r, VkMemoryRequirements *req, VkMemoryPropertyFlags flags)
{
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = req->size;
	memoryAllocateInfo.memoryTypeIndex = FindMemoryType(r, req->memoryTypeBits, flags);

	VkDeviceMemory memory;
	VK_CHECK(vkAllocateMemory(r->device, &memoryAllocateInfo, NULL, &memory));
	return memory;
}

static void CreateBufferWithMemory(
	Render *r, VkBufferCreateInfo *ci, VkMemoryPropertyFlags flags,
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
	commandBufferAllocateInfo.commandPool = r->command_pool;
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

void EndOneTimeCmdbuf(Render *r, VkCommandBuffer cmd_buffer)
{
	VK_CHECK(vkEndCommandBuffer(cmd_buffer));

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmd_buffer;

	VK_CHECK(vkQueueSubmit(r->graphics_queue, 1, &submitInfo, VK_NULL_HANDLE));
	VK_CHECK(vkQueueWaitIdle(r->graphics_queue));
	vkFreeCommandBuffers(r->device, r->command_pool, 1, &cmd_buffer);
}

static void CopyBuffer(Render *r, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = BeginOneTimeCmdbuf(r);
	VkBufferCopy copy = {0, 0, size};

	vkCmdCopyBuffer(commandBuffer, src_buffer, dst_buffer, 1, &copy);
	EndOneTimeCmdbuf(r, commandBuffer);
}

static void StageBuffer(Render *r, VkBuffer buffer, void *data, size_t size)
{
	VkBufferCreateInfo stagingCreateInfo = {};
	stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingCreateInfo.size = size;
	stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	CreateBufferWithMemory(r, &stagingCreateInfo,
	                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
	                          &stagingBufferMemory, &stagingBuffer);

	void *stagingBufferPtr;
	VK_CHECK(vkMapMemory(r->device, stagingBufferMemory, 0, stagingCreateInfo.size, 0, &stagingBufferPtr));

	memcpy(stagingBufferPtr, data, size);

	vkUnmapMemory(r->device, stagingBufferMemory);

	CopyBuffer(r, stagingBuffer, buffer, size);

	vkDestroyBuffer(r->device, stagingBuffer, nullptr);
	vkFreeMemory(r->device, stagingBufferMemory, nullptr);
}

static void CreateStorageBuffer(std::string filename, Render *r)
{
	LoadFont(filename, r);

	VkBufferCreateInfo storageCreateInfo = {};
	storageCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	storageCreateInfo.size = r->glyph_data_size;
	storageCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	storageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateBufferWithMemory(r, &storageCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	                          &r->storage_buffer_memory, &r->storage_buffer);

	StageBuffer(r, r->storage_buffer, r->glyph_data, storageCreateInfo.size);

	free(r->glyph_data);
	r->glyph_data = nullptr;
}

static void CreateInstanceBuffer(Render *r)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateBufferWithMemory(r, &bufferCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	                          &r->instance_buffer_memory, &r->instance_buffer);

	VkBufferCreateInfo stagingCreateInfo = {};
	stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	stagingCreateInfo.size = bufferCreateInfo.size;
	stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	CreateBufferWithMemory(r, &stagingCreateInfo,
	                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
	                          &r->instance_staging_buffer_memory, &r->instance_staging_buffer);
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

	VK_CHECK(vkCreateDescriptorPool(r->device, &poolCreateInfo, NULL, &r->descriptor_pool));
}

static void CreateDescriptorSet(Render *r)
{
	VkDescriptorSetAllocateInfo setAllocateInfo = {};
	setAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocateInfo.descriptorPool = r->descriptor_pool;
	setAllocateInfo.descriptorSetCount = 1;
	setAllocateInfo.pSetLayouts = &r->set_layout;

	VK_CHECK(vkAllocateDescriptorSets(r->device, &setAllocateInfo, &r->descriptor_set));

	VkDescriptorBufferInfo glyphInfo = {};
	glyphInfo.buffer = r->storage_buffer;
	glyphInfo.offset = r->glyph_info_offset;
	glyphInfo.range = r->glyph_info_size;

	VkDescriptorBufferInfo cellsInfo = {};
	cellsInfo.buffer = r->storage_buffer;
	cellsInfo.offset = r->glyph_cells_offset;
	cellsInfo.range = r->glyph_cells_size;

	VkDescriptorBufferInfo pointsInfo = {};
	pointsInfo.buffer = r->storage_buffer;
	pointsInfo.offset = r->glyph_points_offset;
	pointsInfo.range = r->glyph_points_size;

	std::array<VkWriteDescriptorSet, 3> writes = {};

	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].dstSet = r->descriptor_set;
	writes[0].dstBinding = 0;
	writes[0].dstArrayElement = 0;
	writes[0].descriptorCount = 1;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[0].pBufferInfo = &glyphInfo;

	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].dstSet = r->descriptor_set;
	writes[1].dstBinding = 1;
	writes[1].dstArrayElement = 0;
	writes[1].descriptorCount = 1;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[1].pBufferInfo = &cellsInfo;

	writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[2].dstSet = r->descriptor_set;
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
		{0, sizeof(GlyphInstance), VK_VERTEX_INPUT_RATE_INSTANCE,};

	VkVertexInputAttributeDescription vertexInputAttributes[] = {
		{0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(GlyphInstance, rect)},
		{1, 0, VK_FORMAT_R32_UINT, offsetof(GlyphInstance, glyph_index)},
		{2, 0, VK_FORMAT_R32_SFLOAT, offsetof(GlyphInstance, sharpness)},
		{3, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(GlyphInstance, colour)},
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
	viewport.width = static_cast<float>(r->swapchain_extent.width);
	viewport.height = static_cast<float>(r->swapchain_extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = r->swapchain_extent;

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
	pipelineInfo.layout = r->pipeline_layout;
	pipelineInfo.renderPass = r->render_pass;
	pipelineInfo.subpass = 0;

	VK_CHECK(vkCreateGraphicsPipelines(
		r->device,
		VK_NULL_HANDLE,
		1,
		&pipelineInfo,
		NULL,
		&r->pipeline));

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

static void CreateVulkanObjects(std::string filename, Render *r)
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

	vkDestroyFence(r->device, r->command_buffer_fence, nullptr);

	vkFreeCommandBuffers(r->device, r->command_pool, 1, &r->command_buffer);

	for (uint32_t i = 0; i < r->image_count; i++)
		vkDestroyFramebuffer(r->device, r->framebuffers[i], nullptr);

	VK_DESTROY(vkDestroyRenderPass, r->device, r->render_pass);

	for (uint32_t i = 0; i < r->image_count; i++)
		vkDestroyImageView(r->device, r->image_views[i], nullptr);
}

static void DestroyVulkanObjects(Render *r)
{
//	vkDeviceWaitIdle(r->device);

	DestroySwapChainObjects(r);

	VK_DESTROY(vkDestroyBuffer, r->device, r->instance_staging_buffer);
	VK_DESTROY(vkFreeMemory, r->device, r->instance_staging_buffer_memory);
	VK_DESTROY(vkDestroyBuffer, r->device, r->instance_buffer);
	VK_DESTROY(vkFreeMemory, r->device, r->instance_buffer_memory);
	VK_DESTROY(vkDestroyBuffer, r->device, r->storage_buffer);
	VK_DESTROY(vkFreeMemory, r->device, r->storage_buffer_memory);
	VK_DESTROY(vkDestroyDescriptorPool, r->device, r->descriptor_pool);
	VK_DESTROY(vkDestroyDescriptorSetLayout, r->device, r->set_layout);
	VK_DESTROY(vkDestroyPipelineLayout, r->device, r->pipeline_layout);
	VK_DESTROY(vkDestroySwapchainKHR, r->device, r->swapchain);
	VK_DESTROY(vkDestroySemaphore, r->device, r->image_available_semaphore);
	VK_DESTROY(vkDestroySemaphore, r->device, r->render_finished_semaphore);
	VK_DESTROY(vkDestroyCommandPool, r->device, r->command_pool);

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
	r->fps_temp++;

	auto curr = static_cast<float>(glfwGetTime());

	if (floorf(curr) > floorf(r->time))
	{
		r->fps = r->fps_temp;
		r->fps_temp = 0;
	}

	r->delta_time = (float)(curr - r->time);
	r->time = curr;

	double xpos, ypos;
	glfwGetCursorPos(r->window, &xpos, &ypos);

	r->mouse_pos.m_x = static_cast<float>(xpos);
	r->mouse_pos.m_y = static_cast<float>(ypos);

	Vector2 mouseDelta = r->old_mouse_pos - r->mouse_pos;

	if (glfwGetMouseButton(r->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		mouseDelta *= 1.0f / r->canvas_scale;
		r->canvas_offset += mouseDelta;
	}

	r->old_mouse_pos = r->mouse_pos;

	Vector2 swapchainExtent = {
		static_cast<float>(r->swapchain_extent.width),
		static_cast<float>(r->swapchain_extent.height)
	};

	Vector2 oldSize = swapchainExtent * (1.0f / r->canvas_scale);

	if (r->canvas_scale != r->target_canvas_scale)
	{
		r->canvas_scale = Maths::Lerp(r->canvas_scale, r->target_canvas_scale, r->delta_time * 30.0f);

		Vector2 newSize = swapchainExtent * (1.0f / r->canvas_scale);
		Vector2 tmp = oldSize - newSize;

		tmp.m_x *= r->mouse_pos.m_x / swapchainExtent.m_x;
		tmp.m_y *= r->mouse_pos.m_y / swapchainExtent.m_y;

		r->canvas_offset += tmp;
	}
}

static void RenderFrame(Render *r)
{
	VkResult res = vkAcquireNextImageKHR(r->device, r->swapchain, std::numeric_limits<uint32_t>::max(),
	                                     r->image_available_semaphore, nullptr, &r->image_index);

	if (res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain(r);
		return;
	}
	if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR)
	{
		return;
	}

	VkFence currentFence = r->command_buffer_fence;
	vkWaitForFences(r->device, 1, &currentFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	vkResetFences(r->device, 1, &currentFence);

	RecordCommandBuffer(r);

	VkSemaphore waitSemaphores[] = {r->image_available_semaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	VkSemaphore signalSemaphores[] = {r->render_finished_semaphore};

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &r->command_buffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	res = vkQueueSubmit(r->graphics_queue, 1, &submitInfo, currentFence);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &r->swapchain;
	presentInfo.pImageIndices = &r->image_index;

	res = vkQueuePresentKHR(r->present_queue, &presentInfo);

	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
	{
		RecreateSwapChain(r);
		//return true;
	}
	else
	{
		//return res == VK_SUCCESS;
	}
}

static void OnWindowResized(GLFWwindow *window, int width, int height)
{
	if (width == 0 || height == 0)
		return;

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

	r->target_canvas_scale *= powf(1.3f, (float)yoffset);
}

int main(int argc, const char **args)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);

	Render render = {};
	render.canvas_scale = 3.0f;
	render.target_canvas_scale = render.canvas_scale;
	render.window = window;

	glfwSetWindowUserPointer(window, &render);
	glfwSetWindowSizeCallback(window, OnWindowResized);
	glfwSetWindowRefreshCallback(window, WindowRefreshCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetScrollCallback(window, ScrollCallback);

//	std::string filename = "Resources/Engine/Fonts/Alice-Regular.ttf";
//	std::string filename = "Resources/Engine/Fonts/marediv.ttf";
	std::string filename = "Resources/Engine/Fonts/Lobster-Regular.ttf";
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
