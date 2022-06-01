#pragma once

#include "Resources/Resource.hpp"
#include "Graphics/Images/Image2dArray.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "Ui/Export.hpp"

typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_ *FT_Face;

namespace acid {
/**
 * @brief Resource that is used when creating a font mesh.
 */
class ACID_UI_EXPORT FontType : public Resource {
public:
	class Glyph {
	public:
		double advance;
		double x, y, w, h;
		double pxRange;
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
	explicit FontType(std::filesystem::path filename, std::size_t size = 24, bool load = true);

	~FontType();

	std::optional<Glyph> GetGlyph(wchar_t ascii) const;
	
	std::type_index GetTypeIndex() const override { return typeid(FontType); }

	bool IsOpen() const noexcept { return library != nullptr; }

	const std::filesystem::path &GetFilename() const { return filename; }
	const Image2dArray *GetImage() const { return image.get(); }
	std::size_t GetSize() const { return size; }

	float GetMaxHeight() const { return maxHeight; }
	float GetMaxAdvance() const { return maxAdvance; }

	friend const Node &operator>>(const Node &node, FontType &fontType);
	friend Node &operator<<(Node &node, const FontType &fontType);

private:
	void Open();
	void Close();
	void Load();

	FT_Library library = nullptr;
	FT_Face face = nullptr;
	
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
