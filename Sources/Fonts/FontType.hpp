#pragma once

#include "Resources/Resource.hpp"
#include "Graphics/Images/Image2d.hpp"
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
			m_leftBearing(leftBearing),
			m_advance(advance),
			m_i0(i0),
			m_i1(i1) {
		}

		int32_t m_leftBearing;
		int32_t m_advance;
		Vector2i m_i0, m_i1;
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

	const std::filesystem::path &GetFilename() const { return m_filename; }

	const std::shared_ptr<Image2d> &GetImage() const { return m_image; }

	float GetMaxHeight() const { return m_maxHeight; }
	float GetMaxAdvance() const { return m_maxAdvance; }

	friend const Node &operator>>(const Node &node, FontType &fontType);
	friend Node &operator<<(Node &node, const FontType &fontType);

private:
	void Load();

	std::filesystem::path m_filename;
	std::shared_ptr<Image2d> m_image;

	/// Char to glyphs index.
	std::map<char, std::size_t> m_indices;
	std::vector<Glyph> m_glyphs;
	/// Glyph size in pixels.
	std::size_t m_size;
	
	float m_maxHeight = 0.0f, m_maxAdvance = 0.0f;
};
}
