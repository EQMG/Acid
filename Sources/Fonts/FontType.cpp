#include "FontType.hpp"

#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"
#include "msdf.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace acid {
static constexpr std::string_view CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890 \"!`?\'.,;:()[]{}<>|/@\\^$-%+=#_&~*\n";

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
	m_filename(std::move(filename)),
	m_size(size) {
	if (load) {
		FontType::Load();
	}
}

const Node &operator>>(const Node &node, FontType &fontType) {
	node["filename"].Get(fontType.m_filename);
	node["size"].Get(fontType.m_size);
	return node;
}

Node &operator<<(Node &node, const FontType &fontType) {
	node["filename"].Set(fontType.m_filename);
	node["size"].Set(fontType.m_size);
	return node;
}

void FontType::Load() {
	if (m_filename.empty()) {
		return;
	}
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	auto bytes = Files::ReadBytes(m_filename);
	stbtt_fontinfo fontinfo;
	stbtt_InitFont(&fontinfo, bytes.data(), stbtt_GetFontOffsetForIndex(bytes.data(), 0));

	auto layerCount = CHARACTERS.size();

	std::size_t index = 0;
	for (auto c : CHARACTERS) {
		ex_metrics_t metrics = {};
		auto bitmap = ex_msdf_glyph(&fontinfo, ex_utf8(&c), m_size, m_size, &metrics);
		if (bitmap) {

			
			free(bitmap);
		} else {
			// This will most likely be space or return characters, advance will still be loaded into it's metrics.
			Log::Warning("Unable to load character: ", (uint32_t)c, "\n");
		}

		m_glyphs.emplace_back(Glyph(metrics.left_bearing, metrics.advance, {metrics.ix0, metrics.iy0}, {metrics.ix1, metrics.iy1}));
		m_indices[c] = index++;
	}

	m_image = std::make_unique<Image2dArray>(Vector2ui(m_size, m_size), layerCount);

#if defined(ACID_DEBUG)
	Log::Out("Font Type ", m_filename, " loaded ", m_glyphs.size(), " glyphs in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

std::optional<FontType::Glyph> FontType::GetGlyph(wchar_t ascii) const {
	auto it = m_indices.find(ascii);

	if (it != m_indices.end()) {
		return m_glyphs[it->second];
	}

	return std::nullopt;
}
}
