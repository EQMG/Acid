/*#include "RendererFonts2.hpp"

#include <assert.h>

namespace acid
{
	const uint32_t RendererFonts2::MAX_VISIBLE_GLYPHS = 4096;
	const uint32_t RendererFonts2::NUMBER_OF_GLYPHS = 128;

	VertexInput GlyphInstance::GetVertexInput(const uint32_t &binding)
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

		return VertexInput(binding, bindingDescriptions, attributeDescriptions);
	}

	RendererFonts2::RendererFonts2(const GraphicsStage &graphicsStage) :
		IRenderer(graphicsStage),
		m_pipeline(Pipeline(graphicsStage, {"Shaders/Fonts2/Font.vert", "Shaders/Fonts2/Font.frag"}, {GlyphInstance::GetVertexInput()},
			PIPELINE_MODE_POLYGON, PIPELINE_DEPTH_READ_WRITE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, {})),
		m_descriptorSet(DescriptorsHandler()),
		m_storageBuffer(StorageHandler()),
		m_outlines(new acid::Outline[NUMBER_OF_GLYPHS]),
		m_glyph_infos(new HostGlyphInfo[NUMBER_OF_GLYPHS])
	{
		LoadFont();
	//	create_instance_buffer();
	//	Update();
		Log::Out("%s\n", m_pipeline.GetShaderProgram()->ToString().c_str());
	}

	void RendererFonts2::Update()
	{
		CommandBuffer commandBuffer = CommandBuffer();

		BeginText();

		AppendText(5.0f, Display::Get()->GetHeight() - 10.0f, 0.02f, "Frame time: " + String::To(static_cast<int>(1000.0f * Engine::Get()->GetDeltaRender().AsSeconds())) + " ms");
		AppendText(5.0f, Display::Get()->GetHeight() - 40.0f, 0.02f, "FPS: " + String::To(static_cast<int>(1.0f / Engine::Get()->GetDeltaRender().AsSeconds())));

		static const std::vector<std::string> lines = {
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
			"Suspendisse ut scelerisque tellus. In ac quam sem.Curabitur suscipit massa nisl. Ut et metus sed lacus dapibus molestie. nullptram",
			"porttitor sit amet magna quis dapibus. nullptra tincidunt, arcu sit amet hendrerit consequat, felis leo blandit libero, eu posuere",
			"nisl quam interdum nullptra. Quisque nec efficitur libero. Quisque quis orci vitae metus feugiat aliquam eu et nullptra. Etiam aliquet",
			"ante vitae lacus aliquam, et gravida elit mollis. Proin molestie, justo tempus rhoncus aliquam, tellus erat venenatis erat,",
			"porttitor dapibus nunc purus id enim. Integer a nunc ut velit porta maximus. nullptram rutrum nisi in sagittis pharetra. Proin id",
			"pharetra augue, sed vulputate lorem. Aenean dapibus, turpis nec ullamcorper pharetra, ex augue congue nibh, condimentum",
			"vestibulum arcu urna quis ex.",
			"",
			"Vestibulum non dignissim nibh, quis vestibulum odio. Ut sed viverra ante, fringilla convallis tellus. Donec in est rutrum,",
			"imperdiet dolor a, vestibulum magna. In nec justo tellus. Ut non erat eu leo ornare imperdiet in sit amet lorem. nullptram quis",
			"nisl diam. Aliquam laoreet dui et ligula posuere cursus.",
			"",
			"Donec vestibulum ante eget arcu dapibus lobortis.Curabitur condimentum tellus felis, id luctus mi ultrices quis. Aenean nullptra",
			"justo, venenatis vel risus et, suscipit faucibus nullptra. Pellentesque habitant morbi tristique senectus et netus et malesuada",
			"fames ac turpis egestas. Sed lacinia metus eleifend lacinia blandit.Morbi est nibh, dapibus nec arcu quis, volutpat lacinia",
			"dolor. Vestibulum quis viverra erat.Maecenas ultricies odio neque, et eleifend arcu auctor in. Suspendisse placerat massa nisl,",
			"non condimentum ligula sodales at.Phasellus eros urna, elementum in ultricies quis, vulputate id magna. Donec efficitur rutrum",
			"urna sed tempus. Vestibulum eu augue dolor. Vestibulum vehicula suscipit purus, sit amet ultricies ligula malesuada sit amet.",
			"Duis consectetur elit euismod arcu aliquet vehicula. Pellentesque lobortis dui et nisl vehicula, in placerat quam dapibus.Fusce",
			"auctor arcu a purus bibendum, eget blandit nisi lobortis.",
		};

		for (uint32_t i = 0; i < lines.size(); i++)
		{
			auto canvasScale = 3.0f;
			auto canvasOffset = Vector2::ZERO;

			AppendText(canvasScale * (10.0f - canvasOffset.m_x),
			           canvasScale * (30.0f - canvasOffset.m_y + i * 30.0f),
			           0.02f * canvasScale,
			           lines[i]);
		}


		EndText(commandBuffer);

		commandBuffer.End();
		commandBuffer.Submit();
	}

	void RendererFonts2::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const Camera &camera)
	{
		m_storageBuffer.Stage(m_glyph_data, 0, m_glyph_data_size);

		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.Push("GlyphBuffer", m_storageBuffer, OffsetSize(m_glyph_info_offset, m_glyph_data_size));
		m_descriptorSet.Push("CellBuffer", m_storageBuffer, OffsetSize(m_glyph_cells_offset, m_glyph_cells_size));
		m_descriptorSet.Push("PointBuffer", m_storageBuffer, OffsetSize(m_glyph_points_offset, m_glyph_points_size));
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
		//	return false;
			return;
		}

		// Draws the instanced objects.
		m_descriptorSet.BindDescriptor(commandBuffer);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 1, &m_instance_buffer, offsets);
		vkCmdDraw(commandBuffer.GetCommandBuffer(), 4, m_glyph_instance_count, 0, 0);
	}

	uint32_t RendererFonts2::align_uint32(const uint32_t &value, const uint32_t &alignment)
	{
		return (value + alignment - 1) / alignment * alignment;
	}

	void RendererFonts2::LoadFont()
	{
		FT_Library library;
		FT_CHECK(FT_Init_FreeType(&library));

		FT_Face face;
//	    NewFace(&library, "Fonts/Pacifico-Regular.ttf", 0, &face);
//	    NewFace(&library, "Fonts/Lato-Medium.ttf", 0, &face);
//	    NewFace(&library, "Fonts/Lobster-Regular.ttf", 0, &face);
		NewFace(&library, "Fonts/LobsterTwo-Italic.ttf", 0, &face);
//	    NewFace(&library, "Fonts/OpenSans-Regular.ttf", 0, &face);

		FT_CHECK(FT_Set_Char_Size(face, 0, 1000 * 64, 96, 96));

		uint32_t total_points = 0;
		uint32_t total_cells = 0;

		for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
		{
			char c = ' ' + i;
		//	printf("%c", c);

			acid::Outline *o = &m_outlines[i];
			HostGlyphInfo *hgi = &m_glyph_infos[i];

			FT_UInt glyph_index = FT_Get_Char_Index(face, c);
			FT_CHECK(FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_HINTING));

			o->Convert(&face->glyph->outline);

			hgi->bbox = o->m_bbox;
			hgi->advance = face->glyph->metrics.horiAdvance / 64.0f;

			total_points += o->m_pointCount;
			total_cells += o->m_cellCountX * o->m_cellCountY;
		}

		m_glyph_info_size = sizeof(DeviceGlyphInfo) * NUMBER_OF_GLYPHS;
		m_glyph_cells_size = sizeof(uint32_t) * total_cells;
		m_glyph_points_size = sizeof(acid::Vector2) * total_points;

		auto alignment = static_cast<uint32_t>(Display::Get()->GetPhysicalDeviceProperties().limits.minStorageBufferOffsetAlignment);
		m_glyph_info_offset = 0;
		m_glyph_cells_offset = align_uint32(m_glyph_info_size, alignment);
		m_glyph_points_offset = align_uint32(m_glyph_info_size + m_glyph_cells_size, alignment);
		m_glyph_data_size = m_glyph_points_offset + m_glyph_points_size;

		m_glyph_data = malloc(m_glyph_data_size);

		DeviceGlyphInfo *device_glyph_infos = (DeviceGlyphInfo*)((char*)m_glyph_data + m_glyph_info_offset);
		uint32_t *cells = (uint32_t*)((char*)m_glyph_data + m_glyph_cells_offset);
		acid::Vector2 *points = (acid::Vector2*)((char*)m_glyph_data + m_glyph_points_offset);

		uint32_t point_offset = 0;
		uint32_t cell_offset = 0;

		for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
		{
			acid::Outline *o = &m_outlines[i];
			DeviceGlyphInfo *dgi = &device_glyph_infos[i];

			dgi->cell_info.cell_count_x = o->m_cellCountX;
			dgi->cell_info.cell_count_y = o->m_cellCountY;
			dgi->cell_info.point_offset = point_offset;
			dgi->cell_info.cell_offset = cell_offset;
			dgi->bbox = o->m_bbox;

			uint32_t cell_count = o->m_cellCountX * o->m_cellCountY;
			memcpy(cells + cell_offset, o->m_cells, sizeof(uint32_t) * cell_count);
			memcpy(points + point_offset, o->m_points, sizeof(acid::Vector2) * o->m_pointCount);

			point_offset += o->m_pointCount;
			cell_offset += cell_count;
		}

		assert(point_offset == total_points);
		assert(cell_offset == total_cells);

		for (uint32_t i = 0; i < NUMBER_OF_GLYPHS; i++)
		{
			m_outlines[i].Destroy();
		}

		FT_CHECK(FT_Done_Face(face));
		FT_CHECK(FT_Done_FreeType(library));
	}

	void RendererFonts2::NewFace(FT_Library *library, const std::string &filename, const signed long &faceIndex, FT_Face *aface)
	{
		auto fileLoaded = Files::Read(filename);
		assert(fileLoaded);
		FT_CHECK(FT_New_Memory_Face(*library, (FT_Byte*)fileLoaded->data(), (FT_Long)fileLoaded->size(), faceIndex, aface));
	}

	void RendererFonts2::create_instance_buffer()
	{
		VkBufferCreateInfo ci = {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance),
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		};

		create_buffer_with_memory(&ci, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		                          &m_instance_buffer_memory, &m_instance_buffer);

		VkBufferCreateInfo staging_ci = {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = ci.size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		};

		create_buffer_with_memory(&staging_ci,
		                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
		                          &m_instance_staging_buffer_memory, &m_instance_staging_buffer);
	}

	VkDeviceMemory RendererFonts2::alloc_required_memory(VkMemoryRequirements *req, VkMemoryPropertyFlags flags)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		VkMemoryAllocateInfo alloc_info = {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = req->size,
			.memoryTypeIndex = Buffer::FindMemoryType(req->memoryTypeBits, flags),
		};

		VkDeviceMemory mem;
		Display::CheckVk(vkAllocateMemory(logicalDevice, &alloc_info, nullptr, &mem));
		return mem;
	}

	void RendererFonts2::create_buffer_with_memory(VkBufferCreateInfo *ci, VkMemoryPropertyFlags flags,
	                                               VkDeviceMemory *memory, VkBuffer *buffer)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		Display::CheckVk(vkCreateBuffer(logicalDevice, ci, nullptr, buffer));

		VkMemoryRequirements req;
		vkGetBufferMemoryRequirements(logicalDevice, *buffer, &req);

		*memory = alloc_required_memory(&req, flags);
		Display::CheckVk(vkBindBufferMemory(logicalDevice, *buffer, *memory, 0));
	}

	void RendererFonts2::BeginText()
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		m_glyph_instance_count = 0;
		uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

		Display::CheckVk(vkMapMemory(logicalDevice, m_instance_staging_buffer_memory, 0, size, 0, (void **)&m_glyph_instances));
	}

	void RendererFonts2::AppendText(float x, float y, float scale, const std::string &text)
	{
		for (char c : text)
		{
			if (m_glyph_instance_count >= MAX_VISIBLE_GLYPHS)
				break;

			uint32_t glyph_index = static_cast<uint32_t>(c) - 32;

			HostGlyphInfo *gi = &m_glyph_infos[glyph_index];
			GlyphInstance *inst = &m_glyph_instances[m_glyph_instance_count];

			inst->rect.minX = (x + gi->bbox.minX * scale) / (Display::Get()->GetWidth() / 2.0f) - 1.0f;
			inst->rect.minY = (y - gi->bbox.minY * scale) / (Display::Get()->GetHeight() / 2.0f) - 1.0f;
			inst->rect.maxX = (x + gi->bbox.maxX * scale) / (Display::Get()->GetWidth() / 2.0f) - 1.0f;
			inst->rect.maxY = (y - gi->bbox.maxY * scale) / (Display::Get()->GetHeight() / 2.0f) - 1.0f;

			if (inst->rect.minX <= 1 && inst->rect.maxX >= -1 &&
			    inst->rect.maxY <= 1 && inst->rect.minY >= -1)
			{
				inst->glyph_index = glyph_index;
				inst->sharpness = scale;

				m_glyph_instance_count++;
			}

			x += gi->advance * scale;
		}
	}

	void RendererFonts2::EndText(const CommandBuffer &commandBuffer)
	{
		auto logicalDevice = Display::Get()->GetLogicalDevice();

		uint32_t size = MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance);

		vkUnmapMemory(logicalDevice, m_instance_staging_buffer_memory);

		VkBufferMemoryBarrier barrier = {
			.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
			.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
			.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
			.buffer = m_instance_buffer,
			.offset = 0,
			.size = size,
		};

		vkCmdPipelineBarrier(
			commandBuffer.GetCommandBuffer(),
			VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			1, &barrier,
			0, nullptr);

		VkBufferCopy copy = {
			.srcOffset = 0,
			.dstOffset = 0,
			.size = size
		};

		vkCmdCopyBuffer(commandBuffer.GetCommandBuffer(), m_instance_staging_buffer, m_instance_buffer, 1, &copy);

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
}*/
