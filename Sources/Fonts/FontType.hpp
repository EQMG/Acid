#pragma once

#include "Resources/Resource.hpp"
#include "Graphics/Images/Image2dArray.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"

namespace acid {
class Text;

/**
 * @brief Resource that is used when creating a font mesh.
 */
class ACID_EXPORT FontType : public Resource {
public:
	class Glyph {
	public:
		Glyph(int32_t leftBearing, int32_t advance, const Vector2i &i0, const Vector2i &i1) :
			leftBearing(leftBearing),
			advance(advance),
			i0(i0),
			i1(i1) {
		}

		int32_t leftBearing;
		int32_t advance;
		Vector2i i0, i1;
	};
	
	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const Node &node);

	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param filename The font file to load glyphs for this type from.
	 * @param size The size of each glyph in pixels.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const std::filesystem::path &filename, std::size_t size = 24);

	/**
	 * Creates a new font type.
	 * @param filename The font file to load glyphs for this type from.
	 * @param size The size of each glyph in pixels.
	 * @param load If this resource will be loaded immediately, otherwise {@link FontType#Load} can be called later.
	 */
	FontType(std::filesystem::path filename, std::size_t size = 24, bool load = true);

	std::optional<Glyph> GetGlyph(wchar_t ascii) const;
	
	std::type_index GetTypeIndex() const override { return typeid(FontType); }

	const std::filesystem::path &GetFilename() const { return filename; }
	const Image2dArray *GetImage() const { return image.get(); }
	std::size_t GetSize() const { return size; }

	float GetMaxHeight() const { return maxHeight; }
	float GetMaxAdvance() const { return maxAdvance; }

	friend const Node &operator>>(const Node &node, FontType &fontType);
	friend Node &operator<<(Node &node, const FontType &fontType);

private:
	void Load();

	std::filesystem::path filename;
	std::unique_ptr<Image2dArray> image;

	/// Char to glyphs index.
	std::map<wchar_t, std::size_t> indices;
	std::vector<Glyph> glyphs;
	/// Glyph size in pixels.
	std::size_t size;
	
	float maxHeight = 0.0f, maxAdvance = 0.0f;
};
}
