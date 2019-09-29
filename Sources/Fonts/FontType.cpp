#include "FontType.hpp"

#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
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

std::shared_ptr<FontType> FontType::Create(const std::filesystem::path &filename, const std::string &style) {
	FontType temp(filename, style, false);
	Node node;
	node << temp;
	return Create(node);
}

FontType::FontType(std::filesystem::path filename, std::string style, bool load) :
	m_filename(std::move(filename)),
	m_style(std::move(style)) {
	if (load) {
		FontType::Load();
	}
}

const Node &operator>>(const Node &node, FontType &fontType) {
	node["filename"].Get(fontType.m_filename);
	node["style"].Get(fontType.m_style);
	return node;
}

Node &operator<<(Node &node, const FontType &fontType) {
	node["filename"].Set(fontType.m_filename);
	node["style"].Set(fontType.m_style);
	return node;
}

void FontType::Load() {
	if (m_filename.empty() || m_style.empty()) {
		return;
	}

	m_image = Image2d::Create(m_filename / (m_style + ".png"));
	m_node = std::make_unique<FontMetafile>(m_filename / (m_style + ".fnt"));
}
}
