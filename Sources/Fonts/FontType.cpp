#include "FontType.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"
#include "Files/Json/Json.hpp"
#include "Files/Files.hpp"
#include "Files/File.hpp"
#include "Helpers/Enumerate.hpp"
#include "Devices/Window.hpp"
#include "Text.hpp"

namespace acid {
static const uint32_t MAX_VISIBLE_GLYPHS = 4096;

std::shared_ptr<FontType> FontType::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find(node)) {
		return std::dynamic_pointer_cast<FontType>(resource);
	}

	auto result = std::make_shared<FontType>("", "");
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<FontType> FontType::Create(const std::filesystem::path &filename) {
	FontType temp(filename, false);
	Node node;
	node << temp;
	return Create(node);
}

FontType::FontType(std::filesystem::path filename, bool load) :
	m_filename(std::move(filename)) {
	if (load) {
		FontType::Load();
	}
}

const Node &operator>>(const Node &node, FontType &fontType) {
	node["filename"].Get(fontType.m_filename);
	return node;
}

Node &operator<<(Node &node, const FontType &fontType) {
	node["filename"].Set(fontType.m_filename);
	return node;
}

void FontType::Update(const std::vector<Text *> &texts) {
	m_instances = 0;

	if (texts.empty()) return;

	Instance *instances;
	m_instanceBuffer->MapMemory(reinterpret_cast<void **>(&instances));

	auto extent = Window::Get()->GetSize();

	for (const auto &text : texts) {
		auto fontSize = text->GetFontSize() * text->GetScreenScale() / 1000.0f;
		//auto size = text->GetScreenTransform().GetSize();
		auto position = text->GetScreenTransform().GetPosition();

		Vector2f localOffset;

		for (auto c : text->GetString()) {
			if (m_instances >= MAX_VISIBLE_GLYPHS) {
				break;
			}

			auto glyphIndex = m_charmap[c];
			auto gi = &m_glyphInfos[glyphIndex];
			auto instance = &instances[m_instances];

			if (c == '\n') {
				localOffset.m_x = 0.0f;
				localOffset.m_y += gi->vertAdvance * fontSize.m_y;
				continue;
			}
			if (localOffset.m_x + (gi->horiAdvance * fontSize.m_x) >= text->GetScreenTransform().GetSize().m_x) {
				localOffset.m_x = 0.0f;
				localOffset.m_y += gi->vertAdvance * fontSize.m_y;
			}

			instance->m_rect.m_min.m_x = ((position.m_x + localOffset.m_x) + gi->bbox.m_min.m_x * fontSize.m_x) / (extent.m_x / 2.0f) - 1.0f;
			instance->m_rect.m_min.m_y = ((position.m_y + localOffset.m_y) - gi->bbox.m_min.m_y * fontSize.m_y) / (extent.m_y / 2.0f) - 1.0f;
			instance->m_rect.m_max.m_x = ((position.m_x + localOffset.m_x) + gi->bbox.m_max.m_x * fontSize.m_x) / (extent.m_x / 2.0f) - 1.0f;
			instance->m_rect.m_max.m_y = ((position.m_y + localOffset.m_y) - gi->bbox.m_max.m_y * fontSize.m_y) / (extent.m_y / 2.0f) - 1.0f;

			if (instance->m_rect.m_min.m_x <= 1.0f && instance->m_rect.m_max.m_x >= -1.0f && instance->m_rect.m_max.m_y <= 1.0f && instance->m_rect.m_min.m_y >= -1.0f) {
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

bool FontType::CmdRender(const CommandBuffer &commandBuffer, const PipelineGraphics &pipeline) {
	if (m_instances == 0) return false;

	// Updates descriptors.
	m_descriptorSet.Push("BufferGlyph", *m_storageGlyphs, OffsetSize(m_glyphInfoOffset, m_glyphInfoSize));
	m_descriptorSet.Push("BufferCell", *m_storageGlyphs, OffsetSize(m_glyphCellsOffset, m_glyphCellsSize));
	m_descriptorSet.Push("BufferPoint", *m_storageGlyphs, OffsetSize(m_glyphPointsOffset, m_glyphPointsSize));

	if (!m_descriptorSet.Update(pipeline))
		return false;

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, pipeline);

	VkBuffer buffers[1] = {m_instanceBuffer->GetBuffer()};
	VkDeviceSize offsets[1] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	vkCmdDraw(commandBuffer, 4, m_instances, 0, 0);
	return true;
}

void FontType::Load() {
	if (m_filename.empty()) return;

#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	auto physicalDevice = Graphics::Get()->GetPhysicalDevice();

	auto fileLoaded = Files::Read(m_filename);

	if (!fileLoaded) {
		Log::Error("Could not load font ", m_filename, '\n');
		return;
	}

	FT_Library library;
	if (FT_Init_FreeType(&library) != 0)
		throw std::runtime_error("Freetype failed to initialize");

	FT_Face face;
	if (FT_New_Memory_Face(library, reinterpret_cast<FT_Byte *>(fileLoaded->data()), static_cast<FT_Long>(fileLoaded->size()), 0, &face) != 0)
		throw std::runtime_error("Freetype failed to create face from memory");

	if (FT_Set_Char_Size(face, 0, 1000 * 64, 96, 96) != 0)
		throw std::runtime_error("Freetype failed to set char size");

	uint32_t totalPoints = 0;
	uint32_t totalCells = 0;

	auto glyphCount = static_cast<uint32_t>(face->num_glyphs);
	m_charmap = {};
	m_glyphInfos = std::vector<HostGlyphInfo>(glyphCount);
	std::vector<Outline> outlines(glyphCount);

	FT_UInt glyphIndex;
	auto charcode = FT_Get_First_Char(face, &glyphIndex);
	uint32_t i = 0;
	while (glyphIndex != 0) {
		if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_HINTING) != 0)
			throw std::runtime_error("Freetype failed to load a glyph");

		m_charmap.emplace(static_cast<wchar_t>(charcode), i);
		auto hgi = &m_glyphInfos[i];
		auto o = &outlines[i];

		o->OutlineConvert(&face->glyph->outline);

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

	auto alignment = static_cast<uint32_t>(physicalDevice->GetProperties().limits.minStorageBufferOffsetAlignment);
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
	for (uint32_t j = 0; j < m_glyphInfos.size(); j++) {
		auto o = &outlines[j];
		auto dgi = &deviceGlyphInfos[j];

		dgi->cellInfo.cellCount = o->m_cellCount;
		dgi->cellInfo.pointOffset = pointOffset;
		dgi->cellInfo.cellOffset = cellOffset;
		dgi->bbox = o->m_bbox;

		std::memcpy(cells + cellOffset, o->m_cells.data(), sizeof(uint32_t) * o->m_cells.size());
		std::memcpy(points + pointOffset, o->m_points.data(), sizeof(Vector2f) * o->m_points.size());

		//dgi->cbox = o->GetU16Points(reinterpret_cast<Vector2us *>(reinterpret_cast<char *>(points) + pointOffset));

		pointOffset += static_cast<uint32_t>(o->m_points.size());
		cellOffset += static_cast<uint32_t>(o->m_cells.size());
	}

	m_storageGlyphs->UnmapMemory();

	assert(pointOffset == totalPoints);
	assert(cellOffset == totalCells);

	if (FT_Done_Face(face) != 0)
		throw std::runtime_error("Freetype failed to destory face");

	if (FT_Done_FreeType(library) != 0)
		throw std::runtime_error("Freetype failed to destory library");

#if defined(ACID_DEBUG)
	Log::Out("Font Type ", m_filename, " with ", i, " glyphs loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif

/*#if defined(ACID_DEBUG)
	File file(std::make_unique<Json>());
	for (const auto &[i, glyphInfo] : Enumerate(m_glyphInfos)) {
		for (auto [c, j] : m_charmap) {
			if (j != i) continue;

			auto &node = (*file.GetNode())["glyphInfos"]->AddProperty(std::string(1, c));

			node["id"] = i;
			node["bbox"]["min"] = glyphInfo.bbox.m_min;
			node["bbox"]["max"] = glyphInfo.bbox.m_max;
			node["horiAdvance"] = glyphInfo.horiAdvance;
			node["vertAdvance"] = glyphInfo.vertAdvance;

			auto o = &outlines[i];
			node["outline"]["points"] = o->m_points;
			for (const auto &contours : o->m_contours) {
				auto &cont = node["outline"]["contours"]->AddProperty();
				cont["begin"] = contours.m_begin;
				cont["end"] = contours.m_end;
			}
			node["outline"]["cells"] = o->m_cells;
		}
	}
	file.Write("FontsInfo/" + m_filename.stem().string() + ".json", Node::Format::Beautified);
#endif*/
}

uint32_t FontType::AlignUint32(uint32_t value, uint32_t alignment) {
	return (value + alignment - 1) / alignment * alignment;
}
}
