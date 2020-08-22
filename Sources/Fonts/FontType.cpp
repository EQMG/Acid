#include "FontType.hpp"

#include <tinymsdf/tiny_msdf.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
static const std::wstring_view NEHE = L" \t\r\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890\"!`?'.,;:()[]{}<>|/@\\^$-%+=#_&~*";

template<typename T>
constexpr double F26DOT6_TO_DOUBLE(T x) {
	return 1.0 / 64.0 * double(x);
}

std::shared_ptr<FontType> FontType::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<FontType>(node))
		return resource;

	auto result = std::make_shared<FontType>("", 0, false);
	Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
	node >> *result;
	result->Load();
	return result;
}

std::shared_ptr<FontType> FontType::Create(const std::filesystem::path &filename, std::size_t size) {
	FontType temp(filename, size, false);
	Node node;
	node << temp;
	return Create(node);
}

FontType::FontType(std::filesystem::path filename, std::size_t size, bool load) :
	filename(std::move(filename)),
	size(size) {
	if (load)
		FontType::Load();
}

FontType::~FontType() {
	Close();
}

const Node &operator>>(const Node &node, FontType &fontType) {
	node["filename"].Get(fontType.filename);
	node["size"].Get(fontType.size);
	return node;
}

Node &operator<<(Node &node, const FontType &fontType) {
	node["filename"].Set(fontType.filename);
	node["size"].Set(fontType.size);
	return node;
}

void FontType::Open() {
	if (auto error = FT_Init_FreeType(&library))
		throw std::runtime_error("Freetype failed to initialize");

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Font could not be loaded: ", filename, '\n');
		return;
	}

	if (FT_New_Memory_Face(library, reinterpret_cast<FT_Byte *>(fileLoaded->data()), static_cast<FT_Long>(fileLoaded->size()), 0, &face) != 0)
		throw std::runtime_error("Freetype failed to create face from memory");

	// Multiply pixel size by 64 as FreeType uses points instead of pixels internally.
	if (FT_Set_Char_Size(face, size * 64, size * 64, 96, 96) != 0)
		throw std::runtime_error("Freetype failed to set char size");
}

void FontType::Close() {
	if (!IsOpen())
		return;
	FT_Done_Face(face);
	face = nullptr;
	FT_Done_FreeType(library);
	library = nullptr;
}

void FontType::Load() {
	if (filename.empty()) return;

#ifdef ACID_DEBUG
	auto debugStart = Time::Now();
#endif

	Open();
	
	auto layerCount = NEHE.size();
	//image = std::make_unique<Image2dArray>(Vector2ui(size, size), layerCount, VK_FORMAT_R32G32B32A32_SFLOAT);

	tinymsdf::Bitmap<float, 4> mtsdf(size, size);
	glyphs.resize(glyphs.size() + layerCount);
	
	for (auto c : NEHE) {
		bool success = !tinymsdf::GenerateMTSDF(mtsdf, face, c);

		auto id = indices.size();
		indices[c] = id;

		glyphs[id].advance = F26DOT6_TO_DOUBLE(face->glyph->metrics.horiAdvance);
		glyphs[id].x = F26DOT6_TO_DOUBLE(face->glyph->metrics.horiBearingX);
		glyphs[id].y = F26DOT6_TO_DOUBLE(face->glyph->metrics.horiBearingY);
		glyphs[id].w = F26DOT6_TO_DOUBLE(face->glyph->metrics.width);
		glyphs[id].h = F26DOT6_TO_DOUBLE(face->glyph->metrics.height);
		glyphs[id].pxRange = 2;

		//if (success)
		//	image->SetPixels(mtsdf.pixels, id);
	}

	Close();

#ifdef ACID_DEBUG
	Log::Out("Font Type ", filename, " loaded ", glyphs.size(), " glyphs in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

std::optional<FontType::Glyph> FontType::GetGlyph(wchar_t ascii) const {
	auto it = indices.find(ascii);

	if (it != indices.end()) {
		return glyphs[it->second];
	}

	return std::nullopt;
}
}
