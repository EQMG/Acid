#include "FontMetafile.hpp"

#include "Files/FileSystem.hpp"
#include "Helpers/String.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
FontMetafile::FontMetafile(std::string filename) : m_filename(std::move(filename)), m_verticalPerPixelSize(0.0f), m_horizontalPerPixelSize(0.0f), m_imageWidth(0), m_spaceWidth(0.0f), m_paddingWidth(0), m_paddingHeight(0), m_maxSizeY(0.0f)
{
	IFStream inStream(m_filename);

	size_t lineNum = 0;
	std::string linebuf;

	while(inStream.peek() != -1)
		{
			Files::SafeGetLine(inStream, linebuf);
			lineNum++;

			ProcessNextLine(linebuf);

			if(String::Contains(linebuf, "info"))
				{
					LoadPaddingData();
				}
			else if(String::Contains(linebuf, "common"))
				{
					LoadLineSizes();
				}
			else if(String::Contains(linebuf, "char") && !String::Contains(linebuf, "chars"))
				{
					LoadCharacterData();
				}
		}
}

std::optional<FontMetafile::Character> FontMetafile::GetCharacter(const int32_t& ascii) const
{
	auto it = m_characters.find(ascii);

	if(it != m_characters.end())
		{
			return it->second;
		}

	return {};
}

void FontMetafile::ProcessNextLine(const std::string& line)
{
	m_values.clear();
	auto parts = String::Split(line, " ");

	for(const auto& part : parts)
		{
			auto pairs = String::Split(part, "=");

			if(pairs.size() == 2)
				{
					m_values.emplace(pairs.at(0), pairs.at(1));
				}
		}
}

void FontMetafile::LoadPaddingData()
{
	for(const auto& padding : GetValuesOfVariable("padding"))
		{
			m_padding.emplace_back(padding);
		}

	m_paddingWidth = m_padding.at(PadLeft) + m_padding.at(PadRight);
	m_paddingHeight = m_padding.at(PadTop) + m_padding.at(PadBottom);
}

void FontMetafile::LoadLineSizes()
{
	int32_t lineHeightPixels = GetValueOfVariable("lineHeight") - m_paddingHeight;
	m_verticalPerPixelSize = LineHeight / static_cast<float>(lineHeightPixels);
	m_horizontalPerPixelSize = m_verticalPerPixelSize;
	m_imageWidth = GetValueOfVariable("scaleW");
}

void FontMetafile::LoadCharacterData()
{
	auto id = GetValueOfVariable("id");

	if(id == SpaceAscii)
		{
			m_spaceWidth = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
			return;
		}

	auto xTextureCoord = (static_cast<float>(GetValueOfVariable("x")) + (m_padding.at(PadLeft) - DesiredPassing)) / m_imageWidth;
	auto yTextureCoord = (static_cast<float>(GetValueOfVariable("y")) + (m_padding.at(PadTop) - DesiredPassing)) / m_imageWidth;
	auto width = GetValueOfVariable("width") - (m_paddingWidth - (2 * DesiredPassing));
	auto height = GetValueOfVariable("height") - ((m_paddingHeight) - (2 * DesiredPassing));
	auto quadWidth = width * m_horizontalPerPixelSize;
	auto quadHeight = height * m_verticalPerPixelSize;
	auto xTexSize = static_cast<float>(width) / m_imageWidth;
	auto yTexSize = static_cast<float>(height) / m_imageWidth;
	auto xOffset = (GetValueOfVariable("xoffset") + m_padding.at(PadLeft) - DesiredPassing) * m_horizontalPerPixelSize;
	auto yOffset = (GetValueOfVariable("yoffset") + (m_padding.at(PadTop) - DesiredPassing)) * m_verticalPerPixelSize;
	auto xAdvance = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;

	if(quadHeight > m_maxSizeY)
		{
			m_maxSizeY = quadHeight;
		}

	auto character = Character(id, xTextureCoord, yTextureCoord, xTexSize, yTexSize, xOffset, yOffset, quadWidth, quadHeight, xAdvance);
	m_characters.emplace(character.m_id, character);
}

int32_t FontMetafile::GetValueOfVariable(const std::string& variable)
{
	return String::From<int32_t>(m_values.at(variable));
}

std::vector<int32_t> FontMetafile::GetValuesOfVariable(const std::string& variable)
{
	std::vector<int32_t> result = {};
	auto numbers = String::Split(m_values.at(variable), ",");
	result.reserve(numbers.size());

	for(const auto& number : numbers)
		{
			result.emplace_back(String::From<int32_t>(number));
		}

	return result;
}
}
