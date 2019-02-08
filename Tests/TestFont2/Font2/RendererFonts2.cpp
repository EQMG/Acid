#include "RendererFonts2.hpp"

#include <Models/VertexModel.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>
#include <Renderer/Renderer.hpp>

namespace acid
{
	Shader::VertexInput RendererFonts2::GlyphInstance::GetVertexInput(const uint32_t &binding)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = binding;
		bindingDescriptions[0].stride = sizeof(GlyphInstance);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

		// Rectangle attribute.
		attributeDescriptions[0].binding = binding;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(GlyphInstance, rect);

		// Glyph index attribute.
		attributeDescriptions[1].binding = binding;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32_UINT;
		attributeDescriptions[1].offset = offsetof(GlyphInstance, glyph_index);

		// Sharpness attribute.
		attributeDescriptions[2].binding = binding;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(GlyphInstance, sharpness);

		return Shader::VertexInput(binding, bindingDescriptions, attributeDescriptions);

	}

	RendererFonts2::RendererFonts2(const Pipeline::Stage &pipelineStage) :
		RenderPipeline(pipelineStage),
		m_pipeline(PipelineGraphics(pipelineStage, {"Shaders/Fonts2/Font.vert", "Shaders/Fonts2/Font.frag"}, {GlyphInstance::GetVertexInput()},
			PipelineGraphics::Mode::Polygon, PipelineGraphics::Depth::None, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, false, {})),
		m_descriptorSet(DescriptorsHandler()),
		m_storageBuffer(StorageHandler(true)),
		outlines(new Outline[NUMBER_OF_GLYPHS]),
		glyph_infos(new HostGlyphInfo[NUMBER_OF_GLYPHS]),
		glyph_data(nullptr),
		glyph_data_size(0),
		glyph_info_size(0),
		glyph_cells_size(0),
		glyph_points_size(0),
		glyph_info_offset(0),
		glyph_cells_offset(0),
		glyph_points_offset(0),
		glyph_instances(nullptr),
		glyph_instance_count(0),
		storage_buffer_memory(VK_NULL_HANDLE),
		storage_buffer(VK_NULL_HANDLE),
		instance_buffer_memory(VK_NULL_HANDLE),
		instance_buffer(VK_NULL_HANDLE),
		instance_staging_buffer_memory(VK_NULL_HANDLE),
		instance_staging_buffer(VK_NULL_HANDLE)
	{
		Log::Out("%s\n", m_pipeline.GetShaderProgram()->ToString().c_str());

		load_font();
		create_instance_buffer();

		CommandBuffer commandBuffer = CommandBuffer();
		begin_text();

		append_text(5.0f, 25.0f, 0.02f, "Frame Time: " + String::To(Engine::Get()->GetDelta().AsMilliseconds()) + "ms");
		append_text(5.0f, 55.0f, 0.02f, "Fps: " + String::To(1.0f / Engine::Get()->GetDelta().AsSeconds()));

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
			append_text(30.0f, 90.0f * (1.0f + i), 0.06f, lines[i]);
		}

		end_text(commandBuffer);
		commandBuffer.End();
		commandBuffer.SubmitIdle();
	}

	void RendererFonts2::Render(const CommandBuffer &commandBuffer)
	{
		m_storageBuffer.Push(glyph_data, glyph_data_size);

		m_pipeline.BindPipeline(commandBuffer);

		// Updates descriptors.
		m_descriptorSet.Push("GlyphBuffer", m_storageBuffer, OffsetSize(glyph_info_offset, glyph_data_size));
		m_descriptorSet.Push("CellBuffer", m_storageBuffer, OffsetSize(glyph_cells_offset, glyph_cells_size));
		m_descriptorSet.Push("PointBuffer", m_storageBuffer, OffsetSize(glyph_points_offset, glyph_points_size));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 1, &instance_buffer, offsets);
		vkCmdDraw(commandBuffer.GetCommandBuffer(), 4, glyph_instance_count, 0, 0);
	}

	static uint32_t align_uint32(uint32_t value, uint32_t alignment)
	{
		return (value + alignment - 1) / alignment * alignment;
	}

	void RendererFonts2::load_font()
	{
		FT_Library library;
		FT_CHECK(FT_Init_FreeType(&library));

		FT_Face face;
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/Alice-Regular.ttf", 0, &face));
		FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/Lobster-Regular.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/LobsterTwo-Bold.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/LobsterTwo-BoldItalic.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/LobsterTwo-Italic.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/LobsterTwo-Regular.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-Bold.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-BoldItalic.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-ExtraBold.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-ExtraBoldItalic.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-Italic.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-Light.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-LightItalic.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-Regular.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-SemiBold.ttf", 0, &face));
	//	FT_CHECK(FT_New_Face(library, "Resources/Engine/Fonts/OpenSans-SemiBoldItalic.ttf", 0, &face));

		FT_CHECK(FT_Set_Char_Size(face, 0, 1000 * 64, 96, 96));

		uint32_t total_points = 0;
		uint32_t total_cells = 0;

		for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
		{
			char c = ' ' + i;
			Outline *o = &outlines[i];
			HostGlyphInfo *hgi = &glyph_infos[i];

			FT_UInt glyph_index = FT_Get_Char_Index(face, c);
			FT_CHECK(FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_HINTING));

			outline_convert(&face->glyph->outline, o, c);

			hgi->bbox = o->bbox;
			hgi->advance = face->glyph->metrics.horiAdvance / 64.0f;


			total_points += o->num_of_points;
			total_cells += o->cell_count_x * o->cell_count_y;
		}

		glyph_info_size = sizeof(DeviceGlyphInfo) * NUMBER_OF_GLYPHS;
		glyph_cells_size = sizeof(uint32_t) * total_cells;
		glyph_points_size = sizeof(Vector2) * total_points;

		uint32_t alignment = Renderer::Get()->GetPhysicalDevice()->GetProperties().limits.minStorageBufferOffsetAlignment;
		glyph_info_offset = 0;
		glyph_cells_offset = align_uint32(glyph_info_size, alignment);
		glyph_points_offset = align_uint32(glyph_info_size + glyph_cells_size, alignment);
		glyph_data_size = glyph_points_offset + glyph_points_size;

		glyph_data = malloc(glyph_data_size);

		DeviceGlyphInfo *device_glyph_infos = (DeviceGlyphInfo*)((char*)glyph_data + glyph_info_offset);
		uint32_t *cells = (uint32_t*)((char*)glyph_data + glyph_cells_offset);
		Vector2 *points = (Vector2*)((char*)glyph_data + glyph_points_offset);

		uint32_t point_offset = 0;
		uint32_t cell_offset = 0;

		for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
		{
			Outline *o = &outlines[i];
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
			outline_destroy(&outlines[i]);

		FT_CHECK(FT_Done_Face(face));
		FT_CHECK(FT_Done_FreeType(library));
	}

	VkDeviceMemory RendererFonts2::alloc_required_memory(VkMemoryRequirements *req, VkMemoryPropertyFlags flags)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		VkMemoryAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = req->size;
		alloc_info.memoryTypeIndex = Buffer::FindMemoryType(req->memoryTypeBits, flags);		

		VkDeviceMemory mem;
		Renderer::CheckVk(vkAllocateMemory(logicalDevice->GetLogicalDevice(), &alloc_info, NULL, &mem));
		return mem;
	}

	void RendererFonts2::create_buffer_with_memory(VkBufferCreateInfo *ci, VkMemoryPropertyFlags flags, VkDeviceMemory *memory, VkBuffer *buffer)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		Renderer::CheckVk(vkCreateBuffer(logicalDevice->GetLogicalDevice(), ci, NULL, buffer));

		VkMemoryRequirements req;
		vkGetBufferMemoryRequirements(logicalDevice->GetLogicalDevice(), *buffer, &req);

		*memory = alloc_required_memory(&req, flags);
		Renderer::CheckVk(vkBindBufferMemory(logicalDevice->GetLogicalDevice(), *buffer, *memory, 0));
	}

	void RendererFonts2::create_instance_buffer()
	{
		VkBufferCreateInfo ci = {};
		ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		ci.size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);
		ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		
		create_buffer_with_memory(&ci, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		                          &instance_buffer_memory, &instance_buffer);

		VkBufferCreateInfo staging_ci = {};
		staging_ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		staging_ci.size = ci.size;
		staging_ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		staging_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		

		create_buffer_with_memory(&staging_ci,
		                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		                          &instance_staging_buffer_memory, &instance_staging_buffer);
	}

	void RendererFonts2::begin_text()
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		glyph_instance_count = 0;

		uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

		Renderer::CheckVk(vkMapMemory(logicalDevice->GetLogicalDevice(), instance_staging_buffer_memory, 0, size, 0, (void **)&glyph_instances));
	}

	void RendererFonts2::end_text(const CommandBuffer &commandBuffer)
	{
		auto logicalDevice = Renderer::Get()->GetLogicalDevice();

		uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

		vkUnmapMemory(logicalDevice->GetLogicalDevice(), instance_staging_buffer_memory);

		VkBufferMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		barrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.buffer = instance_buffer;
		barrier.offset = 0;
		barrier.size = size;
		
		vkCmdPipelineBarrier(
			commandBuffer.GetCommandBuffer(),
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

		vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), instance_staging_buffer, instance_buffer, 1, &copy);

		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;

		vkCmdPipelineBarrier(
			commandBuffer.GetCommandBuffer(),
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
			0,
			0, nullptr,
			1, &barrier,
			0, nullptr);
	}

	void RendererFonts2::append_text(float x, float y, float scale, std::string text)
	{
		for (char c : text)
		{
			if (glyph_instance_count >= MAX_VISIBLE_GLYPHS)
				break;

			uint32_t glyph_index = c - 32;

			HostGlyphInfo *gi = &glyph_infos[glyph_index];
			GlyphInstance *inst = &glyph_instances[glyph_instance_count];

			inst->rect.min_x = (x + gi->bbox.min_x * scale) / (static_cast<float>(Window::Get()->GetWidth()) / 2.0f) - 1.0f;
			inst->rect.min_y = (y - gi->bbox.min_y * scale) / (static_cast<float>(Window::Get()->GetHeight()) / 2.0f) - 1.0f;
			inst->rect.max_x = (x + gi->bbox.max_x * scale) / (static_cast<float>(Window::Get()->GetWidth()) / 2.0f) - 1.0f;
			inst->rect.max_y = (y - gi->bbox.max_y * scale) / (static_cast<float>(Window::Get()->GetHeight()) / 2.0f) - 1.0f;

			if (inst->rect.min_x <= 1 && inst->rect.max_x >= -1 &&
			    inst->rect.max_y <= 1 && inst->rect.min_y >= -1)
			{
				inst->glyph_index = glyph_index;
				inst->sharpness = scale;

				glyph_instance_count++;
			}

			x += gi->advance * scale;
		}
	}
}
