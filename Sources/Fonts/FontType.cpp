#include "FontType.hpp"

#include <msdf/msdf.h>
#include <stb/stb_truetype.h>

#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
static const std::wstring_view NEHE = L" \t\r\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890\"!`?'.,;:()[]{}<>|/@\\^$-%+=#_&~*";

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

	auto layerCount = NEHE.size();
	//m_image = std::make_unique<Image2dArray>(Vector2ui(m_size, m_size), layerCount, VK_FORMAT_R32G32B32_SFLOAT);

	std::size_t arrayLayer = 0;
	for (auto c : NEHE) {
		ex_metrics_t metrics = {};
		auto bitmap = ex_msdf_glyph(&fontinfo, c, m_size, m_size, &metrics, 1);
		if (bitmap) {
			//m_image->SetPixels(bitmap, arrayLayer);

			free(bitmap);
		}

		m_glyphs.emplace_back(Glyph(metrics.left_bearing, metrics.advance, {metrics.ix0, metrics.iy0}, {metrics.ix1, metrics.iy1}));
		m_indices[c] = arrayLayer++;
	}

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
