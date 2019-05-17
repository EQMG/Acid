#include "FontType.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"
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
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<FontType> FontType::Create(const std::string &filename, const std::string &style)
{
	auto temp = FontType(filename, style, false);
	temp.m_filename = filename;
	temp.m_style = style;
	Metadata metadata = Metadata();
	metadata << temp;
	return Create(metadata);
}

FontType::FontType(std::string filename, std::string style, const bool &load) :
	m_filename(std::move(filename)),
	m_style(std::move(style)),
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

	Instance *instances;
	m_instanceBuffer->MapMemory(reinterpret_cast<void **>(&instances));

	for (const auto &text : texts)
	{
		Vector2f extent = Window::Get()->GetSize();
		auto scale = text->GetScreenScale() / 64.0f;
		//auto dimensions = 2.0f * text->GetScreenDimensions();
		auto position = extent * text->GetScreenPosition(); // 2.0f *text->GetScreenPosition() - 1.0f;

		Vector2f localOffset = Vector2f();

		for (const auto &c : text->GetString())
		{
			if (m_instances >= MAX_VISIBLE_GLYPHS)
			{
				break;
			}

			const uint32_t glyphIndex = m_charmap[c];
			const HostGlyphInfo *gi = &m_glyphInfos[glyphIndex];
			auto instance = &instances[m_instances];

			if (c == '\n')
			{
				localOffset.m_x = 0.0f;
				localOffset.m_y += gi->vertAdvance * scale.m_y;
				continue;
			}

			instance->m_rect.minX = ((position.m_x + localOffset.m_x) + gi->bbox.minX * scale.m_x) / (extent.m_x / 2.0f) - 1.0f;
			instance->m_rect.minY = ((position.m_y + localOffset.m_y) - gi->bbox.minY * scale.m_y) / (extent.m_y / 2.0f) - 1.0f;
			instance->m_rect.maxX = ((position.m_x + localOffset.m_x) + gi->bbox.maxX * scale.m_x) / (extent.m_x / 2.0f) - 1.0f;
			instance->m_rect.maxY = ((position.m_y + localOffset.m_y) - gi->bbox.maxY * scale.m_y) / (extent.m_y / 2.0f) - 1.0f;

			//inst->rect.minX = ((x + local.m_x) + gi->bbox.minX * localScale) / (extent.m_x / 2.0f) - 1.0f;
			//inst->rect.minY = ((y + local.m_y) - gi->bbox.minY * localScale) / (extent.m_y / 2.0f) - 1.0f;
			//inst->rect.maxX = ((x + local.m_x) + gi->bbox.maxX * localScale) / (extent.m_x / 2.0f) - 1.0f;
			//inst->rect.maxY = ((y + local.m_y) - gi->bbox.maxY * localScale) / (extent.m_y / 2.0f) - 1.0f;

			if (instance->m_rect.minX <= 1.0f && instance->m_rect.maxX >= -1.0f && instance->m_rect.maxY <= 1.0f && instance->m_rect.minY >= -1.0f)
			{
				instance->m_glyphIndex = glyphIndex;
				instance->m_sharpness = scale.m_x;
				instance->m_colour = text->m_textColour;

				m_instances++;
			}

			localOffset.m_x += gi->horiAdvance * scale.m_x;
		}
	}

	m_instanceBuffer->UnmapMemory();
}

bool FontType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline)
{
	if (m_instances == 0)
	{
		return false;
	}

	// Updates descriptors.
	m_descriptorSet.Push("BufferGlyph", *m_storageGlyphs, OffsetSize(m_glyphInfoOffset, m_glyphInfoSize));
	m_descriptorSet.Push("BufferCell", *m_storageGlyphs, OffsetSize(m_glyphCellsOffset, m_glyphCellsSize));
	m_descriptorSet.Push("BufferPoint", *m_storageGlyphs, OffsetSize(m_glyphPointsOffset, m_glyphPointsSize));
	bool updateSuccess = m_descriptorSet.Update(pipeline);

	if (!updateSuccess)
	{
		return false;
	}

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_instanceBuffer->GetBuffer(), offsets);
	vkCmdDraw(commandBuffer, 4, m_instances, 0, 0);
	return true;
}

void FontType::Load()
{
	if (m_filename.empty() || m_style.empty())
	{
		return;
	}

	m_image = Image2d::Create(m_filename + "/" + m_style + ".png");
	m_metadata = std::make_unique<FontMetafile>(m_filename + "/" + m_style + ".fnt");
	LoadFont(m_filename + "/" + m_style + ".ttf");
}

const Metadata &operator>>(const Metadata &metadata, FontType &fontType)
{
	metadata.GetChild("Filename", fontType.m_filename);
	metadata.GetChild("Style", fontType.m_style);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const FontType &fontType)
{
	metadata.SetChild("Filename", fontType.m_filename);
	metadata.SetChild("Style", fontType.m_style);
	return metadata;
}

uint32_t FontType::AlignUint32(const uint32_t &value, const uint32_t &alignment)
{
	return (value + alignment - 1) / alignment * alignment;
}

void FontType::LoadFont(const std::string &filename)
{
	auto physicalDevice = Graphics::Get()->GetPhysicalDevice();

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
	m_glyphPointsSize = sizeof(Vector2f) * totalPoints;

	uint32_t alignment = static_cast<uint32_t>(physicalDevice->GetProperties().limits.minStorageBufferOffsetAlignment);
	m_glyphInfoOffset = 0;
	m_glyphCellsOffset = AlignUint32(m_glyphInfoSize, alignment);
	m_glyphPointsOffset = AlignUint32(m_glyphInfoSize + m_glyphCellsSize, alignment);

	m_glyphDataSize = m_glyphPointsOffset + m_glyphPointsSize;
	m_storageGlyphs = std::make_unique<StorageBuffer>(m_glyphDataSize);
	m_instanceBuffer = std::make_unique<InstanceBuffer>(MAX_VISIBLE_GLYPHS * sizeof(Instance));

	char *glyphData;
	m_storageGlyphs->MapMemory(reinterpret_cast<void **>(&glyphData));

	auto deviceGlyphInfos = reinterpret_cast<DeviceGlyphInfo *>(glyphData + m_glyphInfoOffset);
	auto cells = reinterpret_cast<uint32_t *>(glyphData + m_glyphCellsOffset);
	auto points = reinterpret_cast<Vector2f *>(glyphData + m_glyphPointsOffset);

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

		std::memcpy(cells + cellOffset, o->cells.data(), sizeof(uint32_t) * o->cells.size());
		std::memcpy(points + pointOffset, o->points.data(), sizeof(Vector2f) * o->points.size());

		//OutlineU16Points(o, &dgi->cbox, reinterpret_cast<PointU16 *>(reinterpret_cast<char *>(points) + pointOffset));

		pointOffset += static_cast<uint32_t>(o->points.size());
		cellOffset += static_cast<uint32_t>(o->cells.size());
	}

	m_storageGlyphs->UnmapMemory();

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
