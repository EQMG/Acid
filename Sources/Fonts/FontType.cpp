#include "FontType.hpp"

#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Graphics/Graphics.hpp"

namespace acid {
std::shared_ptr<FontType> FontType::Create(const Node &node) {
	if (auto resource = Resources::Get()->Find<FontType>(node))
		return resource;

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

void FontType::Load() {
	if (m_filename.empty()) {
		return;
	}

	IFStream inStream(m_filename);

	std::size_t lineNum = 0;
	std::string linebuf;

	while (inStream.peek() != -1) {
		Files::SafeGetLine(inStream, linebuf);
		lineNum++;

		ProcessNextLine(linebuf);

		if (String::StartsWith(linebuf, "info ")) {
			LoadPaddingData();
		} else if (String::StartsWith(linebuf, "common ")) {
			LoadLineSizes();
		} else if (String::StartsWith(linebuf, "page ")) {
			auto id = GetValueOfVariable("id");
			auto file = GetValueOfVariable<std::string>("file");
			file = file.substr(1, file.size() - 2);
			m_image = Image2d::Create(m_filename.parent_path() / file);
		} else if (String::StartsWith(linebuf, "char ")) {
			LoadCharacterData();
		}
	}
}

std::optional<FontType::Character> FontType::GetCharacter(int32_t ascii) const {
	auto it = m_characters.find(ascii);

	if (it != m_characters.end()) {
		return it->second;
	}

	return std::nullopt;
}

void FontType::ProcessNextLine(const std::string &line) {
	m_values.clear();
	auto parts = String::Split(line, ' ');

	for (const auto &part : parts) {
		auto pairs = String::Split(part, '=');

		if (pairs.size() == 2) {
			m_values.emplace(pairs.at(0), pairs.at(1));
		}
	}
}

void FontType::LoadPaddingData() {
	for (const auto &padding : GetValuesOfVariable("padding")) {
		m_padding.emplace_back(padding);
	}

	m_paddingWidth = m_padding.at(PadLeft) + m_padding.at(PadRight);
	m_paddingHeight = m_padding.at(PadTop) + m_padding.at(PadBottom);
}

void FontType::LoadLineSizes() {
	auto lineHeightPixels = GetValueOfVariable("lineHeight") - m_paddingHeight;
	m_verticalPerPixelSize = LineHeight / static_cast<float>(lineHeightPixels);
	m_horizontalPerPixelSize = m_verticalPerPixelSize;
	m_imageWidth = GetValueOfVariable("scaleW");
}

void FontType::LoadCharacterData() {
	auto id = GetValueOfVariable("id");

	if (id == SpaceAscii) {
		m_spaceWidth = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
		return;
	}

	auto xTextureCoord = (GetValueOfVariable<float>("x") + (m_padding.at(PadLeft) - DesiredPassing)) / m_imageWidth;
	auto yTextureCoord = (GetValueOfVariable<float>("y") + (m_padding.at(PadTop) - DesiredPassing)) / m_imageWidth;
	auto width = GetValueOfVariable("width") - (m_paddingWidth - (2 * DesiredPassing));
	auto height = GetValueOfVariable("height") - (m_paddingHeight - (2 * DesiredPassing));
	auto quadWidth = width * m_horizontalPerPixelSize;
	auto quadHeight = height * m_verticalPerPixelSize;
	auto xTexSize = static_cast<float>(width) / m_imageWidth;
	auto yTexSize = static_cast<float>(height) / m_imageWidth;
	auto xOffset = (GetValueOfVariable("xoffset") + m_padding.at(PadLeft) - DesiredPassing) * m_horizontalPerPixelSize;
	auto yOffset = (GetValueOfVariable("yoffset") + (m_padding.at(PadTop) - DesiredPassing)) * m_verticalPerPixelSize;
	auto xAdvance = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
	//auto page = GetValueOfVariable("page");

	m_maxHeight = std::max(quadHeight, m_maxHeight);
	m_maxAdvance = std::max(xAdvance, m_maxAdvance);

	Character character(id, xTextureCoord, yTextureCoord, xTexSize, yTexSize, xOffset, yOffset, quadWidth, quadHeight, xAdvance);
	m_characters.emplace(character.m_id, character);
}

std::vector<int32_t> FontType::GetValuesOfVariable(const std::string &variable) {
	auto numbers = String::Split(m_values.at(variable), ',');

	std::vector<int32_t> values;
	values.reserve(numbers.size());

	for (const auto &number : numbers) {
		values.emplace_back(String::From<int32_t>(number));
	}

	return values;
}
}
