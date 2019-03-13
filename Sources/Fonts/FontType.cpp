#include "FontType.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Resources/Resources.hpp"
#include "Renderer/Renderer.hpp"
#include "Text.hpp"

namespace acid
{
	static const uint32_t MAX_VISIBLE_GLYPHS = 4096;

	std::shared_ptr<FontType> FontType::Create(const Metadata &metadata)
	{
		auto resource = Resources::Get()->Find(metadata);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<FontType>(resource);
		}

		auto result = std::make_shared<FontType>("", "");
		Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
		result->Decode(metadata);
		result->Load();
		return result;
	}

	std::shared_ptr<FontType> FontType::Create(const std::string &filename, const std::string &style)
	{
		auto temp = FontType(filename, style, false);
		temp.m_filename = filename;
		temp.m_style = style;
		Metadata metadata = Metadata();
		temp.Encode(metadata);
		return Create(metadata);
	}

	FontType::FontType(std::string filename, std::string style, const bool &load) :
		m_filename(std::move(filename)),
		m_style(std::move(style)),
		m_texture(nullptr),
		m_metadata(nullptr),
		m_storageGlyphs(nullptr),
		m_instanceBuffer(nullptr),
		m_glyphInstances(nullptr),
		m_instances(0)
	{
		if (load)
		{
			FontType::Load();
		}
	}

	void FontType::Update(const std::vector<Text *> &texts)
	{
		m_instances = 0;

		if (texts.empty())
		{
			return;
		}

		m_instanceBuffer->Map(reinterpret_cast<void **>(&m_glyphInstances));

		for (const auto &text : texts)
		{
			Vector2 extent = Window::Get()->GetDimensions();
			float scale = text->GetScreenScale() / 64.0f;
		//	auto dimensions = 2.0f * text->GetScreenDimensions();
			auto position = extent * text->GetScreenPosition(); // 2.0f *text->GetScreenPosition() - 1.0f;

			Vector2 localOffset = Vector2();

			for (const auto &c : text->GetString())
			{
				if (m_instances >= MAX_VISIBLE_GLYPHS)
				{
					break;
				}

				const uint32_t glyphIndex = m_charmap[c];
				const HostGlyphInfo *gi = &m_glyphInfos[glyphIndex];
				GlyphInstance *inst = &m_glyphInstances[m_instances];

				if (c == '\n')
				{
					localOffset.m_x = 0.0f;
					localOffset.m_y += gi->vertAdvance * scale;
					continue;
				}

				inst->rect.minX = ((position.m_x + localOffset.m_x) + gi->bbox.minX * scale) / (extent.m_x / 2.0f) - 1.0f;
				inst->rect.minY = ((position.m_y + localOffset.m_y) - gi->bbox.minY * scale) / (extent.m_y / 2.0f) - 1.0f;
				inst->rect.maxX = ((position.m_x + localOffset.m_x) + gi->bbox.maxX * scale) / (extent.m_x / 2.0f) - 1.0f;
				inst->rect.maxY = ((position.m_y + localOffset.m_y) - gi->bbox.maxY * scale) / (extent.m_y / 2.0f) - 1.0f;

			//	inst->rect.minX = ((x + local.m_x) + gi->bbox.minX * localScale) / (extent.m_x / 2.0f) - 1.0f;
			//	inst->rect.minY = ((y + local.m_y) - gi->bbox.minY * localScale) / (extent.m_y / 2.0f) - 1.0f;
			//	inst->rect.maxX = ((x + local.m_x) + gi->bbox.maxX * localScale) / (extent.m_x / 2.0f) - 1.0f;
			//	inst->rect.maxY = ((y + local.m_y) - gi->bbox.maxY * localScale) / (extent.m_y / 2.0f) - 1.0f;

				if (inst->rect.minX <= 1.0f && inst->rect.maxX >= -1.0f &&
					inst->rect.maxY <= 1.0f && inst->rect.minY >= -1.0f)
				{
					inst->glyphIndex = glyphIndex;
					inst->sharpness = scale;
					inst->colour = text->GetTextColour();

					m_instances++;
				}

				localOffset.m_x += gi->horiAdvance * scale;
			}
		}

		m_instanceBuffer->Unmap();
	}

	bool FontType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline)
	{
		if (m_instances == 0)
		{
			return false;
		}

		// Updates descriptors.
		m_descriptorSet.Push("GlyphBuffer", *m_storageGlyphs, OffsetSize(m_glyphInfoOffset, m_glyphInfoSize));
		m_descriptorSet.Push("CellBuffer", *m_storageGlyphs, OffsetSize(m_glyphCellsOffset, m_glyphCellsSize));
		m_descriptorSet.Push("PointBuffer", *m_storageGlyphs, OffsetSize(m_glyphPointsOffset, m_glyphPointsSize));
		bool updateSuccess = m_descriptorSet.Update(pipeline);

		if (!updateSuccess)
		{
			return false;
		}

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.GetCommandBuffer(), 0, 1, &m_instanceBuffer->GetBuffer(), offsets);
		vkCmdDraw(commandBuffer.GetCommandBuffer(), 4, m_instances, 0, 0);
		return true;
	}

	void FontType::Load()
	{
		if (m_filename.empty() || m_style.empty())
		{
			return;
		}

		m_texture = Texture::Create(m_filename + "/" + m_style + ".png");
		m_metadata = std::make_unique<FontMetafile>(m_filename + "/" + m_style + ".fnt");
		LoadFont(m_filename + "/" + m_style + ".ttf");
	}

	void FontType::Decode(const Metadata &metadata)
	{
		metadata.GetChild("Filename", m_filename);
		metadata.GetChild("Style", m_style);
	}

	void FontType::Encode(Metadata &metadata) const
	{
		metadata.SetChild("Filename", m_filename);
		metadata.SetChild("Style", m_style);
	}

	Shader::VertexInput FontType::GetVertexInput(const uint32_t &binding)
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		// The vertex input description.
		bindingDescriptions[0].binding = binding;
		bindingDescriptions[0].stride = sizeof(GlyphInstance);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);

		// Rectangle attribute.
		attributeDescriptions[0].binding = binding;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(GlyphInstance, rect);

		// Glyph Index attribute.
		attributeDescriptions[1].binding = binding;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32_UINT;
		attributeDescriptions[1].offset = offsetof(GlyphInstance, glyphIndex);

		// Sharpness attribute.
		attributeDescriptions[2].binding = binding;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(GlyphInstance, sharpness);

		// Colour attribute.
		attributeDescriptions[3].binding = binding;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(GlyphInstance, colour);

		return Shader::VertexInput(binding, bindingDescriptions, attributeDescriptions);
	}

	uint32_t FontType::AlignUint32(const uint32_t &value, const uint32_t &alignment)
	{
		return (value + alignment - 1) / alignment * alignment;
	}

	void FontType::LoadFont(const std::string &filename)
	{
		auto physicalDevice = Renderer::Get()->GetPhysicalDevice();

		auto fileLoaded = Files::Read(filename); // TODO: Use a stream.

		if (!fileLoaded)
		{
			Log::Error("Font could not be loaded: '%s'\n", filename.c_str());
			return;
		}

		FT_Library library;

		if (FT_Init_FreeType(&library) != 0)
		{
			throw std::runtime_error("Freetype failed to initialize");
		}

		FT_Face face;

		if (FT_New_Memory_Face(library, reinterpret_cast<FT_Byte *>(fileLoaded->data()), static_cast<FT_Long>(fileLoaded->size()), 0, &face) != 0)
		{
			throw std::runtime_error("Freetype failed to create face from memory");
		}

		if (FT_Set_Char_Size(face, 0, 1000 * 64, 96, 96) != 0)
		{
			throw std::runtime_error("Freetype failed to set char size");
		}

		uint32_t totalPoints = 0;
		uint32_t totalCells = 0;

		uint32_t glyphCount = face->num_glyphs;
		m_charmap = std::map<wchar_t, uint32_t>();
		m_glyphInfos = std::vector<HostGlyphInfo>(glyphCount);
		std::vector<Outline> outlines(glyphCount);

	//	Log::Out("Glyph Count: %i\n", glyphCount);

		FT_UInt glyphIndex;
		FT_ULong charcode = FT_Get_First_Char(face, &glyphIndex);
		uint32_t i = 0;

		while (glyphIndex != 0)
		{
			if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_HINTING) != 0)
			{
				throw std::runtime_error("Freetype failed to load a glyph");
			}

			m_charmap.emplace(static_cast<wchar_t>(charcode), i);
			HostGlyphInfo *hgi = &m_glyphInfos[i];
			Outline *o = &outlines[i];

			OutlineConvert(&face->glyph->outline, o);

			hgi->bbox = o->bbox;
			hgi->horiAdvance = face->glyph->metrics.horiAdvance / 64.0f;
			hgi->vertAdvance = face->glyph->metrics.vertAdvance / 64.0f;

			totalPoints += static_cast<uint32_t>(o->points.size());
			totalCells += static_cast<uint32_t>(o->cells.size());

			charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
			i++;
		}

		m_glyphInfoSize = sizeof(DeviceGlyphInfo) * static_cast<uint32_t>(m_glyphInfos.size());
		m_glyphCellsSize = sizeof(uint32_t) * totalCells;
		m_glyphPointsSize = sizeof(Vector2) * totalPoints;

		uint32_t alignment = static_cast<uint32_t>(physicalDevice->GetProperties().limits.minStorageBufferOffsetAlignment);
		m_glyphInfoOffset = 0;
		m_glyphCellsOffset = AlignUint32(m_glyphInfoSize, alignment);
		m_glyphPointsOffset = AlignUint32(m_glyphInfoSize + m_glyphCellsSize, alignment);

		m_glyphDataSize = m_glyphPointsOffset + m_glyphPointsSize;
		m_storageGlyphs = std::make_unique<StorageBuffer>(m_glyphDataSize);
		m_instanceBuffer = std::make_unique<InstanceBuffer>(MAX_VISIBLE_GLYPHS * sizeof(GlyphInstance));

		char *glyphData;
		m_storageGlyphs->Map(reinterpret_cast<void **>(&glyphData));

		auto deviceGlyphInfos = reinterpret_cast<DeviceGlyphInfo *>(glyphData + m_glyphInfoOffset);
		auto cells = reinterpret_cast<uint32_t *>(glyphData + m_glyphCellsOffset);
		auto points = reinterpret_cast<Vector2 *>(glyphData + m_glyphPointsOffset);

		uint32_t pointOffset = 0;
		uint32_t cellOffset = 0;

		for (uint32_t j = 0; j < m_glyphInfos.size(); j++)
		{
			Outline *o = &outlines[j];
			DeviceGlyphInfo *dgi = &deviceGlyphInfos[j];

			dgi->cellInfo.cellCountX = o->cellCountX;
			dgi->cellInfo.cellCountY = o->cellCountY;
			dgi->cellInfo.pointOffset = pointOffset;
			dgi->cellInfo.cellOffset = cellOffset;
			dgi->bbox = o->bbox;

			memcpy(cells + cellOffset, o->cells.data(), sizeof(uint32_t) * o->cells.size());
			memcpy(points + pointOffset, o->points.data(), sizeof(Vector2) * o->points.size());

		//	OutlineU16Points(o, &dgi->cbox, reinterpret_cast<PointU16 *>(reinterpret_cast<char *>(points) + pointOffset));

			pointOffset += static_cast<uint32_t>(o->points.size());
			cellOffset += static_cast<uint32_t>(o->cells.size());
		}

		m_storageGlyphs->Unmap();

		assert(pointOffset == totalPoints);
		assert(cellOffset == totalCells);

		if (FT_Done_Face(face) != 0)
		{
			throw std::runtime_error("Freetype failed to destory face");
		}

		if (FT_Done_FreeType(library) != 0)
		{
			throw std::runtime_error("Freetype failed to destory library");
		}
	}
}
