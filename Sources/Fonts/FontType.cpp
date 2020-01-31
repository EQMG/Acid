#include "FontType.hpp"

#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"
#include "msdf.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace acid {
static constexpr std::wstring_view NEHE = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890 \"!`?'.,;:()[]{}<>|/@\\^$-%+=#_&~*\t\r\n";

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

	auto glyphSize = 3 * m_size * m_size;
	auto layerCount = NEHE.size();
	auto bitmapData = std::make_unique<uint8_t[]>(glyphSize * layerCount);

	//m_image = std::make_unique<Image2dArray>(Vector2ui(m_size, m_size), layerCount, VK_FORMAT_R32G32B32_SFLOAT);

	std::size_t arrayLayer = 0;
	for (auto c : NEHE) {
		ex_metrics_t metrics = {};
		auto bitmap = ex_msdf_glyph(&fontinfo, c, m_size, m_size, &metrics);
		if (bitmap) {
			auto bitmapSdfData = std::make_unique<uint8_t[]>(glyphSize);
			for (std::size_t y = 0; y < m_size; y++) {
				for (std::size_t x = 0; x < m_size; x++) {
					auto index = 3 * ((y * m_size) + x);
					bitmapSdfData[index + 0] = (uint8_t)(255 * (bitmap[index + 0] + m_size) / m_size);
					bitmapSdfData[index + 1] = (uint8_t)(255 * (bitmap[index + 1] + m_size) / m_size);
					bitmapSdfData[index + 2] = (uint8_t)(255 * (bitmap[index + 2] + m_size) / m_size);
				}
			}
			std::memcpy(&bitmapData[glyphSize * arrayLayer], bitmapSdfData.get(), glyphSize);
#if 0
			Bitmap bitmapSdf(std::move(bitmapSdfData), {m_size, m_size}, 3);
			bitmapSdf.Write("Fonts" / m_filename.stem() / (std::to_string(index) + ".png"));
#endif

			//m_image->SetPixels(bitmap, arrayLayer);

			free(bitmap);
		}

		m_glyphs.emplace_back(Glyph(metrics.left_bearing, metrics.advance, {metrics.ix0, metrics.iy0}, {metrics.ix1, metrics.iy1}));
		m_indices[c] = arrayLayer++;
	}

	auto bitmap = std::make_unique<Bitmap>(std::move(bitmapData), Vector2ui(m_size, m_size), 3);
	m_image = std::make_unique<Image2dArray>(std::move(bitmap), layerCount, VK_FORMAT_R8G8B8_UNORM);
#if 1
	for (uint32_t layer = 0; layer < layerCount; layer++) {
		auto bitmapSdf = m_image->GetBitmap(0, layer);
		bitmapSdf->Write("Fonts" / m_filename.stem() / (std::to_string(layer) + ".png"));
	}
#endif

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
