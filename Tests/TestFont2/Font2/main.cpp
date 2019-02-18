#if 1
#include <memory>

#include <array>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
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

static uint32_t align_uint32(uint32_t value, uint32_t alignment)
{
	return (value + alignment - 1) / alignment * alignment;
}

static void load_font(const char *font_face, Render *r)
{
	FT_Library library;
	FT_CHECK(FT_Init_FreeType(&library));

	FT_Face face;
	FT_CHECK(FT_New_Face(library, font_face, 0, &face));

	FT_CHECK(FT_Set_Char_Size(face, 0, 1000 * 64, 96, 96));

	uint32_t total_points = 0;
	uint32_t total_cells = 0;

	for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
	{
		char c = ' ' + i;
		Outline *o = &r->outlines[i];
		HostGlyphInfo *hgi = &r->glyph_infos[i];

		FT_UInt glyph_index = FT_Get_Char_Index(face, c);
		FT_CHECK(FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_HINTING));

		outline_convert(&face->glyph->outline, o, c);

		hgi->bbox = o->bbox;
		hgi->advance = face->glyph->metrics.horiAdvance / 64.0f;

		total_points += o->num_of_points;
		total_cells += o->cell_count_x * o->cell_count_y;
	}

	r->glyph_info_size = sizeof(DeviceGlyphInfo) * NUMBER_OF_GLYPHS;
	r->glyph_cells_size = sizeof(uint32_t) * total_cells;
	r->glyph_points_size = sizeof(Vector2) * total_points;

	uint32_t alignment = r->device_properties.limits.minStorageBufferOffsetAlignment;
	r->glyph_info_offset = 0;
	r->glyph_cells_offset = align_uint32(r->glyph_info_size, alignment);
	r->glyph_points_offset = align_uint32(r->glyph_info_size + r->glyph_cells_size, alignment);
	r->glyph_data_size = r->glyph_points_offset + r->glyph_points_size;

	r->glyph_data = malloc(r->glyph_data_size);

	DeviceGlyphInfo *device_glyph_infos = (DeviceGlyphInfo*)((char*)r->glyph_data + r->glyph_info_offset);
	uint32_t *cells = (uint32_t*)((char*)r->glyph_data + r->glyph_cells_offset);
	Vector2 *points = (Vector2*)((char*)r->glyph_data + r->glyph_points_offset);

	uint32_t point_offset = 0;
	uint32_t cell_offset = 0;

	for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
	{
		Outline *o = &r->outlines[i];
		DeviceGlyphInfo *dgi = &device_glyph_infos[i];

		dgi->cell_info.cell_count_x = o->cell_count_x;
		dgi->cell_info.cell_count_y = o->cell_count_y;
		dgi->cell_info.point_offset = point_offset;
		dgi->cell_info.cell_offset = cell_offset;
		dgi->bbox = o->bbox;

		uint32_t cell_count = o->cell_count_x * o->cell_count_y;
		memcpy(cells + cell_offset, o->cells, sizeof(uint32_t) * cell_count);
		memcpy(points + point_offset, o->points, sizeof(Vector2) * o->num_of_points);

		//outline_u16_points(o, &dgi->cbox, points + point_offset);

		point_offset += o->num_of_points;
		cell_offset += cell_count;
	}

	assert(point_offset == total_points);
	assert(cell_offset == total_cells);

	for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
		outline_destroy(&r->outlines[i]);

	FT_CHECK(FT_Done_Face(face));
	FT_CHECK(FT_Done_FreeType(library));
}

static void create_instance(Render *r)
{
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = "Font Renderer";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	uint32_t requiredInstanceExtensionCount;
	const char **requiredInstanceExtensions = glfwGetRequiredInstanceExtensions(&requiredInstanceExtensionCount);

	VkInstanceCreateInfo instance_info = {};
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pNext = NULL;
	instance_info.flags = 0;
	instance_info.pApplicationInfo = &app_info; 
	instance_info.enabledExtensionCount = requiredInstanceExtensionCount; 
	instance_info.ppEnabledExtensionNames = requiredInstanceExtensions;

	VK_CHECK(vkCreateInstance(&instance_info, NULL, &r->instance));
}

static void create_surface(Render *r)
{
	VK_CHECK(glfwCreateWindowSurface(r->instance, r->window, NULL, &r->surface));
}

static void pick_physical_device(Render *r)
{
	uint32_t physical_device_count;
	VK_CHECK(vkEnumeratePhysicalDevices(r->instance, &physical_device_count, NULL));
	assert(physical_device_count > 0);

	std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
	VK_CHECK(vkEnumeratePhysicalDevices(r->instance, &physical_device_count, physical_devices.data()));

	r->physical_device = physical_devices[0];

	vkGetPhysicalDeviceMemoryProperties(r->physical_device, &r->memory_properties);
	vkGetPhysicalDeviceProperties(r->physical_device, &r->device_properties);
}

static void create_device(Render *r)
{
	char const * extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = NULL;
	device_info.enabledExtensionCount = 1;
	device_info.ppEnabledExtensionNames = extensions;

	float queue_priority = 1.0f;

	if (r->graphics_queue_family == r->present_queue_family)
	{
		VkDeviceQueueCreateInfo queue_info = {};
		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.pNext = NULL;
		queue_info.queueFamilyIndex = r->graphics_queue_family;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = &queue_priority;
		
		device_info.pQueueCreateInfos = &queue_info;
		device_info.queueCreateInfoCount = 1;

		VK_CHECK(vkCreateDevice(r->physical_device, &device_info, NULL, &r->device));
	}
	else
	{
		VkDeviceQueueCreateInfo queue_graphics = {};
		queue_graphics.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_graphics.pNext = NULL;
		queue_graphics.queueFamilyIndex = r->graphics_queue_family;
		queue_graphics.queueCount = 1;
		queue_graphics.pQueuePriorities = &queue_priority;

		VkDeviceQueueCreateInfo queue_present = {};
		queue_present.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_present.pNext = NULL;
		queue_present.queueFamilyIndex = r->present_queue_family;
		queue_present.queueCount = 1;
		queue_present.pQueuePriorities = &queue_priority;


		std::array<VkDeviceQueueCreateInfo, 2> queue_infos = { queue_graphics, queue_present };

		device_info.queueCreateInfoCount = queue_infos.size();
		device_info.pQueueCreateInfos = queue_infos.data();

		VK_CHECK(vkCreateDevice(r->physical_device, &device_info, NULL, &r->device));
	}

	vkGetDeviceQueue(r->device, r->graphics_queue_family, 0, &r->graphics_queue);
	vkGetDeviceQueue(r->device, r->present_queue_family, 0, &r->present_queue);
}

static void find_queue_families(Render *r)
{
	uint32_t family_count;

	vkGetPhysicalDeviceQueueFamilyProperties(r->physical_device, &family_count, NULL);

	std::vector<VkQueueFamilyProperties> properties(family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(r->physical_device, &family_count, properties.data());

	r->graphics_queue_family = std::numeric_limits<uint32_t>::max();
	r->present_queue_family = std::numeric_limits<uint32_t>::max();

	for (uint32_t i = 0; i < family_count; i++)
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
			else
			{
				if (r->graphics_queue_family == std::numeric_limits<uint32_t>::max())
					r->graphics_queue_family = i;
			}
		}
	}

	if (r->present_queue_family == std::numeric_limits<uint32_t>::max())
	{
		for (uint32_t i = 0; i < family_count; i++)
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

static void choose_surface_format(Render *r)
{
	uint32_t format_count;
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
		r->physical_device, r->surface, &format_count, NULL));

	assert(format_count > 0);

	std::vector<VkSurfaceFormatKHR> formats(format_count);
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
		r->physical_device, r->surface, &format_count, formats.data()));
	/*
	if (format_count == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		r->format = VK_FORMAT_B8G8R8A8_UNORM;
	else r->format = formats[0].format;
	*/
	r->format = VK_FORMAT_B8G8R8A8_SRGB;
}

static void choose_present_mode(Render *r)
{
	uint32_t present_mode_count;
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
		r->physical_device, r->surface, &present_mode_count, NULL));

	assert(present_mode_count > 0);

	std::vector<VkPresentModeKHR>	present_modes(present_mode_count);
	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
		r->physical_device, r->surface, &present_mode_count, present_modes.data()));


	for (uint32_t i = 0; i < present_mode_count; i++)
	{
		if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			r->present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			return;
		}
	}

	//r->present_mode = VK_PRESENT_MODE_FIFO_KHR;
	r->present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
}

static void create_swap_chain(Render *r)
{
	VkSurfaceCapabilitiesKHR capabilities;
	VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(r->physical_device, r->surface, &capabilities));

	uint32_t min_image_count = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount && min_image_count > capabilities.maxImageCount)
		min_image_count = capabilities.maxImageCount;

	choose_surface_format(r);
	choose_present_mode(r);

	VkSwapchainKHR old_swapchain = r->swapchain;
	r->swapchain = VK_NULL_HANDLE;

	VkSwapchainCreateInfoKHR swapchain_info = {};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.pNext = NULL;
	swapchain_info.flags = 0;
	swapchain_info.surface = r->surface;
	swapchain_info.minImageCount = min_image_count;
	swapchain_info.imageFormat = r->format;
	swapchain_info.imageColorSpace = r->color_space;
	swapchain_info.imageArrayLayers = 1;
	swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_info.preTransform = capabilities.currentTransform;
	swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info.presentMode = r->present_mode;
	swapchain_info.clipped = VK_TRUE;
	swapchain_info.oldSwapchain = old_swapchain;

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		swapchain_info.imageExtent = capabilities.currentExtent;
	}
	else
	{
		int window_w, window_h;
		glfwGetWindowSize(r->window, &window_w, &window_h);

		uint32_t w, h;
		w = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, (uint32_t)window_w));
		h = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, (uint32_t)window_h));

		swapchain_info.imageExtent.width = w;
		swapchain_info.imageExtent.height = h;
	}

	r->swapchain_extent = swapchain_info.imageExtent;

	if (r->graphics_queue_family != r->present_queue_family)
	{
		uint32_t families[] = { r->graphics_queue_family, r->present_queue_family };
		swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchain_info.queueFamilyIndexCount = 2;
		swapchain_info.pQueueFamilyIndices = families;
	}
	else
	{
		swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_info.queueFamilyIndexCount = 0;
		swapchain_info.pQueueFamilyIndices = NULL;
	}

	VK_CHECK(vkCreateSwapchainKHR(r->device, &swapchain_info, NULL, &r->swapchain));

	if (old_swapchain)
		vkDestroySwapchainKHR(r->device, old_swapchain, NULL);

	VK_CHECK(vkGetSwapchainImagesKHR(r->device, r->swapchain, &r->image_count, NULL));

	r->images.resize(r->image_count);
	VK_CHECK(vkGetSwapchainImagesKHR(r->device, r->swapchain, &r->image_count, r->images.data()));
}

static void create_image_views(Render *r)
{
	r->image_views.resize(r->image_count);

	for (uint32_t i = 0; i < r->image_count; i++)
	{
		VkImageViewCreateInfo ci = {};
		ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ci.image = r->images[i];
		ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ci.format = r->format;
		ci.components = {};
		ci.subresourceRange = {};
		ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ci.subresourceRange.baseMipLevel = 0;
		ci.subresourceRange.levelCount = 1;
		ci.subresourceRange.baseArrayLayer = 0;
		ci.subresourceRange.layerCount = 1;

		VK_CHECK(vkCreateImageView(r->device, &ci, NULL, &r->image_views[i]));
	}
}

static void create_render_pass(Render *r)
{
	VkAttachmentDescription color_attachment = {};
	color_attachment.format = r->format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	std::array<VkAttachmentDescription, 1> attachments = { color_attachment };

	VkAttachmentReference color_attachment_ref = {};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	ci.attachmentCount = attachments.size();
	ci.pAttachments = attachments.data();
	ci.subpassCount = 1;
	ci.pSubpasses = &subpass;
	ci.dependencyCount = 1;
	ci.pDependencies = &dependency;

	VK_CHECK(vkCreateRenderPass(r->device, &ci, NULL, &r->render_pass));
}

static void create_framebuffers(Render *r)
{
	r->framebuffers.resize(r->image_count);

	for (uint32_t i = 0; i < r->image_count; i++)
	{
		VkImageView attachments[] = {
			r->image_views[i],
		};

		VkFramebufferCreateInfo ci = {};
		ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		ci.renderPass = r->render_pass;
		ci.attachmentCount = 1;
		ci.pAttachments = attachments;
		ci.width = r->swapchain_extent.width;
		ci.height = r->swapchain_extent.height;
		ci.layers = 1;

		VK_CHECK(vkCreateFramebuffer(r->device, &ci, NULL, &r->framebuffers[i]));
	}
}

static void create_command_pool(Render *r)
{
	VkCommandPoolCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	ci.queueFamilyIndex = r->graphics_queue_family;

	VK_CHECK(vkCreateCommandPool(r->device, &ci, NULL, &r->command_pool));
}


static void begin_text(Render *r)
{
	r->glyph_instance_count = 0;

	uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

	VK_CHECK(vkMapMemory(r->device, r->instance_staging_buffer_memory, 0, size, 0, (void **)&r->glyph_instances));
}

static void end_text(Render *r)
{
	VkCommandBuffer cmd_buf = r->command_buffer;
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
		cmd_buf,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		0,
		0, NULL,
		1, &barrier,
		0, NULL);

	VkBufferCopy copy = {};
	copy.srcOffset = 0;
	copy.dstOffset = 0;
	copy.size = size;
	
	vkCmdCopyBuffer(cmd_buf, r->instance_staging_buffer, r->instance_buffer, 1, &copy);

	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

	vkCmdPipelineBarrier(
		cmd_buf,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
		0,
		0, NULL,
		1, &barrier,
		0, NULL);
}

static void append_text(Render *r, float x, float y, float scale, std::string text, Colour colour)
{
//	printf("%s\n", text.c_str());
	for (char c : text)
	{
		if (r->glyph_instance_count >= MAX_VISIBLE_GLYPHS)
			break;

		uint32_t glyph_index = c - 32;

		HostGlyphInfo *gi = &r->glyph_infos[glyph_index];
		GlyphInstance *inst = &r->glyph_instances[r->glyph_instance_count];

		inst->rect.min_x = (x + gi->bbox.min_x * scale) / (r->swapchain_extent.width / 2.0f) - 1.0f;
		inst->rect.min_y = (y - gi->bbox.min_y * scale) / (r->swapchain_extent.height / 2.0f) - 1.0f;
		inst->rect.max_x = (x + gi->bbox.max_x * scale) / (r->swapchain_extent.width / 2.0f) - 1.0f;
		inst->rect.max_y = (y - gi->bbox.max_y * scale) / (r->swapchain_extent.height / 2.0f) - 1.0f;

		if (inst->rect.min_x <= 1 && inst->rect.max_x >= -1 &&
			inst->rect.max_y <= 1 && inst->rect.min_y >= -1)
		{
			inst->glyph_index = glyph_index;
			inst->sharpness = scale;
			inst->colour = colour;

			r->glyph_instance_count++;
		}

		x += gi->advance * scale;
	}
}

static void record_command_buffer(Render *r)
{
	VkCommandBuffer cmd_buf = r->command_buffer;

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VK_CHECK(vkBeginCommandBuffer(cmd_buf, &begin_info));

	begin_text(r);

	append_text(r, 5.0f, 25.0f, 0.02f, "Frame Time: " + String::To(1000.0f / r->fps) + "ms", Colour::Red);
	append_text(r, 5.0f, 55.0f, 0.02f, "Fps: " + String::To(r->fps), Colour::Green);

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
		append_text(r,
			r->canvas_scale * (10.0f - r->canvas_offset[0]),
			r->canvas_scale * (30.0f - r->canvas_offset[1] + i * 30.0f),
			0.02f * r->canvas_scale,
			lines[i], Colour::Blue);
	}

	end_text(r);

	VkClearValue clear_value = { 1.0f, 1.0f, 1.0f, 1.0f };
	VkRenderPassBeginInfo render_pass_bi = {};
	render_pass_bi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_bi.renderPass = r->render_pass;
	render_pass_bi.framebuffer = r->framebuffers[r->image_index];
	render_pass_bi.renderArea = {};
	render_pass_bi.renderArea.offset = { 0, 0 },
	render_pass_bi.renderArea.extent = r->swapchain_extent;
	render_pass_bi.clearValueCount = 1;
	render_pass_bi.pClearValues = &clear_value;
	
	vkCmdBeginRenderPass(cmd_buf, &render_pass_bi, VK_SUBPASS_CONTENTS_INLINE);

	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmd_buf, 0, 1, &r->instance_buffer, offsets);

	vkCmdBindDescriptorSets(cmd_buf, VK_PIPELINE_BIND_POINT_GRAPHICS,
		r->pipeline_layout, 0, 1, &r->descriptor_set, 0, NULL);

	vkCmdBindPipeline(cmd_buf, VK_PIPELINE_BIND_POINT_GRAPHICS, r->pipeline);
	vkCmdDraw(cmd_buf, 4, r->glyph_instance_count, 0, 0);

	vkCmdEndRenderPass(cmd_buf);
	VK_CHECK(vkEndCommandBuffer(cmd_buf));
}

static void create_command_buffer(Render *r)
{
	VkCommandBufferAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = r->command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = 1;

	VK_CHECK(vkAllocateCommandBuffers(r->device, &alloc_info, &r->command_buffer));
	record_command_buffer(r);
}

static void create_command_buffer_fence(Render *r)
{
	VkFenceCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	ci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	
	VK_CHECK(vkCreateFence(r->device, &ci, NULL, &r->command_buffer_fence));
}

static void create_semaphores(Render *r)
{
	VkSemaphoreCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VK_CHECK(vkCreateSemaphore(r->device, &ci, NULL, &r->image_available_semaphore));
	VK_CHECK(vkCreateSemaphore(r->device, &ci, NULL, &r->render_finished_semaphore));
}

static VkShaderModule load_shader_module(VkDevice device, const char *path)
{
	FILE *f = fopen(path, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);

	auto code = std::vector<uint32_t>(size / sizeof(uint32_t));
	fread(code.data(), size, 1, f);
	fclose(f);

	VkShaderModuleCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ci.codeSize = size_t(size);
	ci.pCode = code.data();

	VkShaderModule ret;
	VK_CHECK(vkCreateShaderModule(device, &ci, NULL, &ret));

	return ret;
}

static void create_layout(Render *r)
{
	VkDescriptorSetLayoutBinding buffer_glyph_binding = {};
	buffer_glyph_binding.binding = 0;
	buffer_glyph_binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	buffer_glyph_binding.descriptorCount = 1;
	buffer_glyph_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding buffer_cell_binding = {};
	buffer_cell_binding.binding = 1;
	buffer_cell_binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	buffer_cell_binding.descriptorCount = 1;
	buffer_cell_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkDescriptorSetLayoutBinding buffer_point_binding = {};
	buffer_point_binding.binding = 2;
	buffer_point_binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	buffer_point_binding.descriptorCount = 1;
	buffer_point_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 3> bindings = { buffer_glyph_binding, buffer_cell_binding, buffer_point_binding };

	VkDescriptorSetLayoutCreateInfo layout_ci = {};
	layout_ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layout_ci.bindingCount = bindings.size();
	layout_ci.pBindings = bindings.data();
	
	VK_CHECK(vkCreateDescriptorSetLayout(r->device, &layout_ci, NULL, &r->set_layout));

	VkPipelineLayoutCreateInfo pipeline_ci = {};
	pipeline_ci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_ci.setLayoutCount = 1;
	pipeline_ci.pSetLayouts = &r->set_layout;	

	VK_CHECK(vkCreatePipelineLayout(r->device, &pipeline_ci, NULL, &r->pipeline_layout));
}

static uint32_t find_memory_type(Render *r, uint32_t type_bits, VkMemoryPropertyFlags flags)
{
	for (uint32_t i = 0; i < r->memory_properties.memoryTypeCount; i++)
	{
		if (type_bits & 1 << i)
		{
			VkMemoryPropertyFlags f = r->memory_properties.memoryTypes[i].propertyFlags;

			if ((f & flags) == flags)
				return i;
		}
	}

	return std::numeric_limits<uint32_t>::max();
}

static VkDeviceMemory alloc_required_memory(Render *r, VkMemoryRequirements *req, VkMemoryPropertyFlags flags)
{
	VkMemoryAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = req->size;
	alloc_info.memoryTypeIndex = find_memory_type(r, req->memoryTypeBits, flags);	

	VkDeviceMemory mem;
	VK_CHECK(vkAllocateMemory(r->device, &alloc_info, NULL, &mem));
	return mem;
}

static void create_buffer_with_memory(
	Render *r, VkBufferCreateInfo *ci, VkMemoryPropertyFlags flags,
	VkDeviceMemory *memory, VkBuffer *buffer)
{
	VK_CHECK(vkCreateBuffer(r->device, ci, NULL, buffer));

	VkMemoryRequirements req;
	vkGetBufferMemoryRequirements(r->device, *buffer, &req);

	*memory = alloc_required_memory(r, &req, flags);
	VK_CHECK(vkBindBufferMemory(r->device, *buffer, *memory, 0));
}

VkCommandBuffer begin_one_time_cmdbuf(Render *r)
{
	VkCommandBufferAllocateInfo cmd_alloc_info = {};
	cmd_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd_alloc_info.commandPool = r->command_pool;
	cmd_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd_alloc_info.commandBufferCount = 1;	

	VkCommandBuffer cmd_buffer;
	VK_CHECK(vkAllocateCommandBuffers(r->device, &cmd_alloc_info, &cmd_buffer));

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;	

	VK_CHECK(vkBeginCommandBuffer(cmd_buffer, &begin_info));
	return cmd_buffer;
}

void end_one_time_cmdbuf(Render *r, VkCommandBuffer cmd_buffer)
{
	VK_CHECK(vkEndCommandBuffer(cmd_buffer));

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmd_buffer;	

	VK_CHECK(vkQueueSubmit(r->graphics_queue, 1, &submit_info, VK_NULL_HANDLE));
	VK_CHECK(vkQueueWaitIdle(r->graphics_queue));
	vkFreeCommandBuffers(r->device, r->command_pool, 1, &cmd_buffer);
}

static void copy_buffer(Render *r, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size)
{
	VkCommandBuffer cmd_buf = begin_one_time_cmdbuf(r);
	VkBufferCopy copy = { 0, 0, size };

	vkCmdCopyBuffer(cmd_buf, src_buffer, dst_buffer, 1, &copy);
	end_one_time_cmdbuf(r, cmd_buf);
}

static void stage_buffer(Render *r, VkBuffer buffer, void *data, size_t size)
{
	VkBufferCreateInfo staging_ci = {};
	staging_ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	staging_ci.size = size;
	staging_ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	staging_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	

	VkBuffer staging_buffer;
	VkDeviceMemory staging_buffer_memory;

	create_buffer_with_memory(r, &staging_ci,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		&staging_buffer_memory, &staging_buffer);

	void *staging_buffer_ptr;
	VK_CHECK(vkMapMemory(r->device, staging_buffer_memory, 0, staging_ci.size, 0, &staging_buffer_ptr));

	memcpy(staging_buffer_ptr, data, size);

	vkUnmapMemory(r->device, staging_buffer_memory);

	copy_buffer(r, staging_buffer, buffer, size);

	vkDestroyBuffer(r->device, staging_buffer, NULL);
	vkFreeMemory(r->device, staging_buffer_memory, NULL);
}

static void create_storage_buffer(const char *font_face, Render *r)
{
	load_font(font_face, r);

	VkBufferCreateInfo storage_ci = {};
	storage_ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	storage_ci.size = r->glyph_data_size;
	storage_ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	storage_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	

	create_buffer_with_memory(r, &storage_ci, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&r->storage_buffer_memory, &r->storage_buffer);

	stage_buffer(r, r->storage_buffer, r->glyph_data, storage_ci.size);

	free(r->glyph_data);
	r->glyph_data = NULL;
}

static void create_instance_buffer(Render *r)
{
	VkBufferCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	ci.size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);
	ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;	

	create_buffer_with_memory(r, &ci, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&r->instance_buffer_memory, &r->instance_buffer);

	VkBufferCreateInfo staging_ci = {};
	staging_ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	staging_ci.size = ci.size;
	staging_ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	staging_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	create_buffer_with_memory(r, &staging_ci,
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		&r->instance_staging_buffer_memory, &r->instance_staging_buffer);
}

static void create_descriptor_pool(Render *r)
{
	VkDescriptorPoolSize pool_sizes[] = {
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 },
	};

	VkDescriptorPoolCreateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	ci.maxSets = 1;
	ci.poolSizeCount = 1;
	ci.pPoolSizes = pool_sizes;	

	VK_CHECK(vkCreateDescriptorPool(r->device, &ci, NULL, &r->descriptor_pool));
}

static void create_descriptor_set(Render *r)
{
	VkDescriptorSetAllocateInfo alloc_info = {};
	alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	alloc_info.descriptorPool = r->descriptor_pool;
	alloc_info.descriptorSetCount = 1;
	alloc_info.pSetLayouts = &r->set_layout;	

	VK_CHECK(vkAllocateDescriptorSets(r->device, &alloc_info, &r->descriptor_set));

	VkDescriptorBufferInfo glyph_info = {};
	glyph_info.buffer = r->storage_buffer;
	glyph_info.offset = r->glyph_info_offset;
	glyph_info.range = r->glyph_info_size;

	VkDescriptorBufferInfo cells_info = {};
	cells_info.buffer = r->storage_buffer;
	cells_info.offset = r->glyph_cells_offset;
	cells_info.range = r->glyph_cells_size;

	VkDescriptorBufferInfo points_info = {};
	points_info.buffer = r->storage_buffer;
	points_info.offset = r->glyph_points_offset;
	points_info.range = r->glyph_points_size;	

	std::array<VkWriteDescriptorSet, 3> writes = {};

	writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[0].dstSet = r->descriptor_set;
	writes[0].dstBinding = 0;
	writes[0].dstArrayElement = 0;
	writes[0].descriptorCount = 1;
	writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[0].pBufferInfo = &glyph_info;

	writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[1].dstSet = r->descriptor_set;
	writes[1].dstBinding = 1;
	writes[1].dstArrayElement = 0;
	writes[1].descriptorCount = 1;
	writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[1].pBufferInfo = &cells_info;

	writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes[2].dstSet = r->descriptor_set;
	writes[2].dstBinding = 2;
	writes[2].dstArrayElement = 0;
	writes[2].descriptorCount = 1;
	writes[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	writes[2].pBufferInfo = &points_info;

	vkUpdateDescriptorSets(r->device, writes.size(), writes.data(), 0, NULL);
}

static void create_pipeline(Render *r)
{
	VkShaderModule vs_font = load_shader_module(r->device, "shaders/font.vert.spv");
	VkShaderModule fs_font = load_shader_module(r->device, "shaders/font.frag.spv");

	VkPipelineShaderStageCreateInfo vert_shader_stage_info = {};
	vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vert_shader_stage_info.module = vs_font;
	vert_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo frag_shader_stage_info = {};
	frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	frag_shader_stage_info.module = fs_font;
	frag_shader_stage_info.pName = "main";

	VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };

	VkVertexInputBindingDescription vertex_input_binding =
		{ 0, sizeof(GlyphInstance), VK_VERTEX_INPUT_RATE_INSTANCE, };

	VkVertexInputAttributeDescription vertex_input_attributes[] = {
		{ 0, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(GlyphInstance, rect) },
		{ 1, 0, VK_FORMAT_R32_UINT, offsetof(GlyphInstance, glyph_index) },
		{ 2, 0, VK_FORMAT_R32_SFLOAT, offsetof(GlyphInstance, sharpness) },
		{ 3, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(GlyphInstance, colour) },
	};

	VkPipelineVertexInputStateCreateInfo vertex_input_state = {};
	vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_state.vertexBindingDescriptionCount = 1;
	vertex_input_state.pVertexBindingDescriptions = &vertex_input_binding;
	vertex_input_state.vertexAttributeDescriptionCount = 4;
	vertex_input_state.pVertexAttributeDescriptions = vertex_input_attributes;
	
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
	input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	input_assembly_state.primitiveRestartEnable = VK_FALSE;	

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)r->swapchain_extent.width;
	viewport.height = (float)r->swapchain_extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;	

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = r->swapchain_extent;	

	VkPipelineViewportStateCreateInfo viewport_state = {};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer_state = {};
	rasterizer_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer_state.depthClampEnable = VK_FALSE;
	rasterizer_state.rasterizerDiscardEnable = VK_FALSE;
	rasterizer_state.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer_state.cullMode = VK_CULL_MODE_NONE;
	rasterizer_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer_state.depthBiasEnable = VK_FALSE;
	rasterizer_state.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.sampleShadingEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState blend_attachment_state = {};
	blend_attachment_state.blendEnable = VK_TRUE;
	blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
	blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
	blend_attachment_state.alphaBlendOp = VK_BLEND_OP_MAX;
	blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	
	VkPipelineColorBlendStateCreateInfo blend_sate = {};
	blend_sate.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blend_sate.logicOpEnable = VK_FALSE;
	blend_sate.logicOp = VK_LOGIC_OP_COPY;
	blend_sate.attachmentCount = 1;
	blend_sate.pAttachments = &blend_attachment_state;

	VkGraphicsPipelineCreateInfo pipeline_info = {};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;
	pipeline_info.pStages = shader_stages;
	pipeline_info.pVertexInputState = &vertex_input_state;
	pipeline_info.pInputAssemblyState = &input_assembly_state;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer_state;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pColorBlendState = &blend_sate;
	pipeline_info.layout = r->pipeline_layout;
	pipeline_info.renderPass = r->render_pass;
	pipeline_info.subpass = 0;	

	VK_CHECK(vkCreateGraphicsPipelines(
		r->device,
		VK_NULL_HANDLE,
		1,
		&pipeline_info,
		NULL,
		&r->pipeline));

	vkDestroyShaderModule(r->device, vs_font, NULL);
	vkDestroyShaderModule(r->device, fs_font, NULL);
}

static void create_swap_chain_objects(Render *r)
{
	create_image_views(r);
	create_render_pass(r);
	create_pipeline(r);
	create_framebuffers(r);

	create_command_buffer(r);
	create_command_buffer_fence(r);
}

static void create_vulkan_objects(const char *font_face, Render *r)
{
	create_instance(r);
	create_surface(r);
	pick_physical_device(r);
	find_queue_families(r);
	create_device(r);
	create_command_pool(r);
	create_layout(r);
	create_storage_buffer(font_face, r);
	create_instance_buffer(r);
	create_descriptor_pool(r);
	create_descriptor_set(r);

	create_swap_chain(r);
	create_swap_chain_objects(r);
	create_semaphores(r);
}

static void destroy_swap_chain_objects(Render *r)
{
	VK_DESTROY(vkDestroyPipeline, r->device, r->pipeline);

	vkDestroyFence(r->device, r->command_buffer_fence, NULL);

	vkFreeCommandBuffers(r->device, r->command_pool, 1, &r->command_buffer);

	for (uint32_t i = 0; i < r->image_count; i++)
		vkDestroyFramebuffer(r->device, r->framebuffers[i], NULL);

	VK_DESTROY(vkDestroyRenderPass, r->device, r->render_pass);

	for (uint32_t i = 0; i < r->image_count; i++)
		vkDestroyImageView(r->device, r->image_views[i], NULL);
}

static void destroy_vulkan_objects(Render *r)
{
//	vkDeviceWaitIdle(r->device);

	destroy_swap_chain_objects(r);

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

	vkDestroyDevice(r->device, NULL);
	r->device = VK_NULL_HANDLE;

	vkDestroySurfaceKHR(r->instance, r->surface, NULL);
	r->surface = VK_NULL_HANDLE;

	vkDestroyInstance(r->instance, NULL);
	r->instance = VK_NULL_HANDLE;
}

static void recreate_swap_chain(Render *r)
{
	vkDeviceWaitIdle(r->device);

	destroy_swap_chain_objects(r);
	create_swap_chain(r);
	create_swap_chain_objects(r);
}

static void update(Render *r)
{
	r->frame++;
	r->fps_temp++;

	float curr = (float)glfwGetTime();
	if (floorf(curr) > floorf(r->time))
	{
		r->fps = r->fps_temp;
		r->fps_temp = 0;
	}

	r->delta_time = (float)(curr - r->time);
	r->time = curr;

	double xpos, ypos;
	glfwGetCursorPos(r->window, &xpos, &ypos);

	r->mouse_pos.m_x = (float)xpos;
	r->mouse_pos.m_y = (float)ypos;

	Vector2 mouse_delta = r->old_mouse_pos - r->mouse_pos;

	if (glfwGetMouseButton(r->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		mouse_delta *= 1.0f / r->canvas_scale;
		r->canvas_offset += mouse_delta;
	}

	r->old_mouse_pos = r->mouse_pos;

	Vector2 swapchain_extent = {
		(float)r->swapchain_extent.width,
		(float)r->swapchain_extent.height
	};

	Vector2 old_size = swapchain_extent * (1.0f / r->canvas_scale);

	if (r->canvas_scale != r->target_canvas_scale)
	{
		r->canvas_scale = Maths::Lerp(r->canvas_scale, r->target_canvas_scale, r->delta_time * 30.0f);

		Vector2 new_size = swapchain_extent * (1.0f / r->canvas_scale);

		Vector2 tmp = old_size - new_size;

		tmp.m_x *= r->mouse_pos.m_x / swapchain_extent.m_x;
		tmp.m_y *= r->mouse_pos.m_y / swapchain_extent.m_y;

		r->canvas_offset += tmp;
	}
}

static void render_frame(Render *r)
{
	VkResult res = vkAcquireNextImageKHR(r->device, r->swapchain, std::numeric_limits<uint32_t>::max(), 
		r->image_available_semaphore, VK_NULL_HANDLE, &r->image_index);

	if (res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreate_swap_chain(r);
		return;
	}
	else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR)
	{
		return;
	}

	VkFence current_fence = r->command_buffer_fence;
	vkWaitForFences(r->device, 1, &current_fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
	vkResetFences(r->device, 1, &current_fence);

	record_command_buffer(r);

	VkSemaphore wait_semaphores[] = { r->image_available_semaphore };
	VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signal_semaphores[] = { r->render_finished_semaphore };

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = wait_semaphores;
	submit_info.pWaitDstStageMask = wait_stages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &r->command_buffer;
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = signal_semaphores;

	res = vkQueueSubmit(r->graphics_queue, 1, &submit_info, current_fence);

	VkPresentInfoKHR present_info = {};
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = signal_semaphores;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &r->swapchain;
	present_info.pImageIndices = &r->image_index;

	res = vkQueuePresentKHR(r->present_queue, &present_info);

	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
	{
		recreate_swap_chain(r);
		//return true;
	}
	else
	{
		//return res == VK_SUCCESS;
	}
}


static void on_window_resized(GLFWwindow* window, int width, int height)
{
	if (width == 0 || height == 0) return;

	recreate_swap_chain((Render *)(glfwGetWindowUserPointer(window)));
}

void window_refresh_callback(GLFWwindow* window)
{
	Render *r = (Render *)(glfwGetWindowUserPointer(window));

	update(r);
	render_frame(r);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Render *render = (Render *)(glfwGetWindowUserPointer(window));
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Render *r = (Render *)(glfwGetWindowUserPointer(window));

	r->target_canvas_scale *= powf(1.3f, (float)yoffset);
}

int main(int argc, const char **args)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", NULL, NULL);

	Render render = {};
	render.canvas_scale = 3.0f;
	render.target_canvas_scale = render.canvas_scale;
	render.window = window;

	glfwSetWindowUserPointer(window, &render);
	glfwSetWindowSizeCallback(window, on_window_resized);
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

//	const char *font_face = "Resources/Engine/Fonts/Alice-Regular.ttf";
//	const char *font_face = "Resources/Engine/Fonts/marediv.ttf";
	const char *font_face = "Resources/Engine/Fonts/Lobster-Regular.ttf";
//	const char *font_face = "Resources/Engine/Fonts/LobsterTwo-Bold.ttf";
//	const char *font_face = "Resources/Engine/Fonts/LobsterTwo-BoldItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/LobsterTwo-Italic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/LobsterTwo-Regular.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-Bold.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-BoldItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-ExtraBold.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-ExtraBoldItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-Italic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-Light.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-LightItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-Regular.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-SemiBold.ttf";
//	const char *font_face = "Resources/Engine/Fonts/OpenSans-SemiBoldItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-Black.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-BlackItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-Bold.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-BoldItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-Italic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-Light.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-LightItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-Medium.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-MediumItalic.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-Regular.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-Thin.ttf";
//	const char *font_face = "Resources/Engine/Fonts/Roboto-ThinItalic.ttf";

	create_vulkan_objects(font_face, &render);

	while (!glfwWindowShouldClose(window))
	{
		update(&render);
		render_frame(&render);
		glfwPollEvents();
	}

	destroy_vulkan_objects(&render);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#endif
