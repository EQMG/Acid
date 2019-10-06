#pragma once

#include "Resources/Resource.hpp"
#include "Graphics/Images/Image2d.hpp"
#include "Graphics/Pipelines/PipelineGraphics.hpp"
#include "FontMetafile.hpp"

namespace acid {
class Text;

/**
 * @brief Resource that is used when creating a font mesh.
 */
class ACID_EXPORT FontType : public Resource {
public:
	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param node The node to decode values from.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const Node &node);

	/**
	 * Creates a new font type, or finds one with the same values.
	 * @param filename The family file path that the texture atlases and character infos are contained in.
	 * @param style The style postfix to load as this type.
	 * @return The font type with the requested values.
	 */
	static std::shared_ptr<FontType> Create(const std::filesystem::path &filename, const std::string &style = "Regular");

	/**
	 * Creates a new font type.
	 * @param filename The family file path that the texture atlases and character infos are contained in.
	 * @param style The style postfix to load as this type.
	 * @param load If this resource will be loaded immediately, otherwise {@link FontType#Load} can be called later.
	 */
	FontType(std::filesystem::path filename, std::string style, bool load = true);

	const std::shared_ptr<Image2d> &GetImage() const { return m_image; }
	const FontMetafile *GetNode() const { return m_node.get(); }

	friend const Node &operator>>(const Node &node, FontType &fontType);
	friend Node &operator<<(Node &node, const FontType &fontType);

private:
	void Load();
	
	std::filesystem::path m_filename;
	std::string m_style;

	std::shared_ptr<Image2d> m_image;
	std::unique_ptr<FontMetafile> m_node;
};
}
