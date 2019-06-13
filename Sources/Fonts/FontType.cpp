#include "FontType.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"
#include "Text.hpp"

namespace acid
{
static const uint32_t MAX_VISIBLE_GLYPHS{4096};

std::shared_ptr<FontType> FontType::Create(const Metadata &metadata)
{
	auto resource{Resources::Get()->Find(metadata)};

	if (resource != nullptr)
	{
		return std::dynamic_pointer_cast<FontType>(resource);
	}

	auto result{std::make_shared<FontType>("", "")};
	Resources::Get()->Add(metadata, std::dynamic_pointer_cast<Resource>(result));
	metadata >> *result;
	result->Load();
	return result;
}

std::shared_ptr<FontType> FontType::Create(const std::filesystem::path &filename, const std::string &style)
{
	FontType temp{filename, style, false};
	Metadata metadata;
	metadata << temp;
	return Create(metadata);
}

FontType::FontType(std::filesystem::path filename, std::string style, const bool &load) :
	m_filename{std::move(filename)},
	m_style{std::move(style)}
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

	auto extent{Window::Get()->GetSize()};

	for (const auto &text : texts)
	{
		auto fontSize{text->GetFontSize() * text->GetScreenScale() / 1000.0f};
		//auto size{text->GetScreenTransform().GetSize()};
		auto position{text->GetScreenTransform().GetPosition()};

		Vector2f localOffset;

		for (const auto &c : text->GetString())
		{
			if (m_instances >= MAX_VISIBLE_GLYPHS)
			{
				break;
			}

			auto glyphIndex{m_charmap[c]};
			auto gi{&m_glyphInfos[glyphIndex]};
			auto instance{&instances[m_instances]};

			if (c == '\n')
			{
				localOffset.m_x = 0.0f;
				localOffset.m_y += gi->vertAdvance * fontSize.m_y;
				continue;
			}

			instance->m_rect.m_min.m_x = ((position.m_x + localOffset.m_x) + gi->bbox.m_min.m_x * fontSize.m_x) / (extent.m_x / 2.0f) - 1.0f;
			instance->m_rect.m_min.m_y = ((position.m_y + localOffset.m_y) - gi->bbox.m_min.m_y * fontSize.m_y) / (extent.m_y / 2.0f) - 1.0f;
			instance->m_rect.m_max.m_x = ((position.m_x + localOffset.m_x) + gi->bbox.m_max.m_x * fontSize.m_x) / (extent.m_x / 2.0f) - 1.0f;
			instance->m_rect.m_max.m_y = ((position.m_y + localOffset.m_y) - gi->bbox.m_max.m_y * fontSize.m_y) / (extent.m_y / 2.0f) - 1.0f;

			if (instance->m_rect.m_min.m_x <= 1.0f && instance->m_rect.m_max.m_x >= -1.0f && instance->m_rect.m_max.m_y <= 1.0f && instance->m_rect.m_min.m_y >= -1.0f)
			{
				instance->m_glyphIndex = glyphIndex;
				instance->m_sharpness = fontSize.m_x;
				instance->m_colour = text->m_textColour;
				instance->m_colour.m_a *= text->GetScreenAlpha();

				m_instances++;
			}

			localOffset.m_x += gi->horiAdvance * fontSize.m_x;
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

	if (!m_descriptorSet.Update(pipeline))
	{
		return false;
	}

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkDeviceSize offsets[]{ 0 };
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

	m_image = Image2d::Create(m_filename / (m_style + ".png"));
	m_metadata = std::make_unique<FontMetafile>(m_filename / (m_style + ".fnt"));
	LoadFont(m_filename / (m_style + ".ttf"));
}

const Metadata &operator>>(const Metadata &metadata, FontType &fontType)
{
	metadata.GetChild("filename", fontType.m_filename);
	metadata.GetChild("style", fontType.m_style);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const FontType &fontType)
{
	metadata.SetChild("filename", fontType.m_filename);
	metadata.SetChild("style", fontType.m_style);
	return metadata;
}

uint32_t FontType::AlignUint32(const uint32_t &value, const uint32_t &alignment)
{
	return (value + alignment - 1) / alignment * alignment;
}

void FontType::LoadFont(const std::filesystem::path &filename)
{
	auto physicalDevice{Graphics::Get()->GetPhysicalDevice()};

	auto fileLoaded{Files::Read(filename)}; // TODO: Use a stream.

	if (!fileLoaded)
	{
		Log::Error("Font could not be loaded: '%s'\n", filename);
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

	uint32_t totalPoints{};
	uint32_t totalCells{};

	auto glyphCount{static_cast<uint32_t>(face->num_glyphs)};
	m_charmap = {};
	m_glyphInfos = std::vector<HostGlyphInfo>(glyphCount);
	std::vector<Outline> outlines(glyphCount);

	FT_UInt glyphIndex;
	auto charcode{FT_Get_First_Char(face, &glyphIndex)};
	uint32_t i{};

	while (glyphIndex != 0)
	{
		if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_HINTING) != 0)
		{
			throw std::runtime_error("Freetype failed to load a glyph");
		}

		m_charmap.emplace(static_cast<wchar_t>(charcode), i);
		auto hgi{&m_glyphInfos[i]};
		auto o{&outlines[i]};

		OutlineConvert(&face->glyph->outline, o);

		hgi->bbox = o->m_bbox;
		hgi->horiAdvance = face->glyph->metrics.horiAdvance / 64.0f;
		hgi->vertAdvance = face->glyph->metrics.vertAdvance / 64.0f;

		totalPoints += static_cast<uint32_t>(o->m_points.size());
		totalCells += static_cast<uint32_t>(o->m_cells.size());

		charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
		i++;
	}

	m_glyphInfoSize = sizeof(DeviceGlyphInfo) * static_cast<uint32_t>(m_glyphInfos.size());
	m_glyphCellsSize = sizeof(uint32_t) * totalCells;
	m_glyphPointsSize = sizeof(Vector2f) * totalPoints;

	auto alignment{static_cast<uint32_t>(physicalDevice->GetProperties().limits.minStorageBufferOffsetAlignment)};
	m_glyphInfoOffset = 0;
	m_glyphCellsOffset = AlignUint32(m_glyphInfoSize, alignment);
	m_glyphPointsOffset = AlignUint32(m_glyphInfoSize + m_glyphCellsSize, alignment);

	m_glyphDataSize = m_glyphPointsOffset + m_glyphPointsSize;
	m_storageGlyphs = std::make_unique<StorageBuffer>(m_glyphDataSize);
	m_instanceBuffer = std::make_unique<InstanceBuffer>(MAX_VISIBLE_GLYPHS * sizeof(Instance));

	char *glyphData;
	m_storageGlyphs->MapMemory(reinterpret_cast<void **>(&glyphData));

	auto deviceGlyphInfos{reinterpret_cast<DeviceGlyphInfo *>(glyphData + m_glyphInfoOffset)};
	auto cells{reinterpret_cast<uint32_t *>(glyphData + m_glyphCellsOffset)};
	auto points{reinterpret_cast<Vector2f *>(glyphData + m_glyphPointsOffset)};

	uint32_t pointOffset{};
	uint32_t cellOffset{};

	for (uint32_t j{}; j < m_glyphInfos.size(); j++)
	{
		auto o{&outlines[j]};
		auto dgi{&deviceGlyphInfos[j]};

		dgi->cellInfo.cellCount = o->m_cellCount;
		dgi->cellInfo.pointOffset = pointOffset;
		dgi->cellInfo.cellOffset = cellOffset;
		dgi->bbox = o->m_bbox;

		std::memcpy(cells + cellOffset, o->m_cells.data(), sizeof(uint32_t) * o->m_cells.size());
		std::memcpy(points + pointOffset, o->m_points.data(), sizeof(Vector2f) * o->m_points.size());

		//OutlineU16Points(o, &dgi->cbox, reinterpret_cast<Vector2us *>(reinterpret_cast<char *>(points) + pointOffset));

		pointOffset += static_cast<uint32_t>(o->m_points.size());
		cellOffset += static_cast<uint32_t>(o->m_cells.size());
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
