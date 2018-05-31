#include "FontMetafile.hpp"

#include "Helpers/FileSystem.hpp"
#include "Helpers/FormatString.hpp"

namespace fl
{
	const unsigned int FontMetafile::PAD_TOP = 0;
	const unsigned int FontMetafile::PAD_LEFT = 1;
	const unsigned int FontMetafile::PAD_BOTTOM = 2;
	const unsigned int FontMetafile::PAD_RIGHT = 3;
	const int FontMetafile::DESIRED_PADDING = 8;

	const std::string FontMetafile::SPLITTER = " ";
	const std::string FontMetafile::NUMBER_SEPARATOR = ",";

	const double FontMetafile::LINE_HEIGHT = 0.03f;
	const int FontMetafile::SPACE_ASCII = 32;

	FontMetafile::FontMetafile(const std::string &filename) :
		IResource(),
		m_metadata(std::map<int, FontCharacter>()),
		m_values(std::map<std::string, std::string>()),
		m_filename(filename),
		m_verticalPerPixelSize(0.0),
		m_horizontalPerPixelSize(0.0),
		m_imageWidth(0),
		m_spaceWidth(0.0),
		m_padding(std::vector<int>()),
		m_paddingWidth(0),
		m_paddingHeight(0),
		m_maxSizeY(0.0)
	{
		std::string fileLoaded = FileSystem::ReadTextFile(filename);
		auto lines = FormatString::Split(fileLoaded, "\n");

		for (auto line : lines)
		{
			ProcessNextLine(line);

			if (FormatString::Contains(line, "info"))
			{
				LoadPaddingData();
			}
			else if (FormatString::Contains(line, "common"))
			{
				LoadLineSizes();
			}
			else if (FormatString::Contains(line, "char") && !FormatString::Contains(line, "chars"))
			{
				LoadCharacterData();
			}
		}
	}

	FontMetafile::~FontMetafile()
	{
	}

	void FontMetafile::ProcessNextLine(const std::string &line)
	{
		m_values.clear();
		auto parts = FormatString::Split(line, SPLITTER);

		for (auto part : parts)
		{
			auto pairs = FormatString::Split(part, "=");

			if (pairs.size() == 2)
			{
				m_values.emplace(pairs.at(0), pairs.at(1));
			}
		}
	}

	void FontMetafile::LoadPaddingData()
	{
		for (auto padding : GetValuesOfVariable("padding"))
		{
			m_padding.emplace_back(padding);
		}

		m_paddingWidth = m_padding.at(PAD_LEFT) + m_padding.at(PAD_RIGHT);
		m_paddingHeight = m_padding.at(PAD_TOP) + m_padding.at(PAD_BOTTOM);
	}

	void FontMetafile::LoadLineSizes()
	{
		int lineHeightPixels = GetValueOfVariable("lineHeight") - m_paddingHeight;
		m_verticalPerPixelSize = LINE_HEIGHT / static_cast<double>(lineHeightPixels);
		m_horizontalPerPixelSize = m_verticalPerPixelSize;
		m_imageWidth = GetValueOfVariable("scaleW");
	}

	void FontMetafile::LoadCharacterData()
	{
		int id = GetValueOfVariable("id");

		if (id == SPACE_ASCII)
		{
			m_spaceWidth = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
			return;
		}

		double xTextureCoord = (static_cast<double>(GetValueOfVariable("x")) + (m_padding.at(PAD_LEFT) - DESIRED_PADDING)) / m_imageWidth;
		double yTextureCoord = (static_cast<double>(GetValueOfVariable("y")) + (m_padding.at(PAD_TOP) - DESIRED_PADDING)) / m_imageWidth;
		int width = GetValueOfVariable("width") - (m_paddingWidth - (2 * DESIRED_PADDING));
		int height = GetValueOfVariable("height") - ((m_paddingHeight) - (2 * DESIRED_PADDING));
		double quadWidth = width * m_horizontalPerPixelSize;
		double quadHeight = height * m_verticalPerPixelSize;
		double xTexSize = static_cast<double>(width) / m_imageWidth;
		double yTexSize = static_cast<double>(height) / m_imageWidth;
		double xOffset = (GetValueOfVariable("xoffset") + m_padding.at(PAD_LEFT) - DESIRED_PADDING) * m_horizontalPerPixelSize;
		double yOffset = (GetValueOfVariable("yoffset") + (m_padding.at(PAD_TOP) - DESIRED_PADDING)) * m_verticalPerPixelSize;
		double xAdvance = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;

		if (quadHeight > m_maxSizeY)
		{
			m_maxSizeY = quadHeight;
		}

		FontCharacter character = FontCharacter(id, xTextureCoord, yTextureCoord, xTexSize, yTexSize, xOffset, yOffset, quadWidth, quadHeight, xAdvance);
		m_metadata.emplace(character.GetId(), character);
	}

	int FontMetafile::GetValueOfVariable(const std::string &variable)
	{
		return std::stoi(m_values.at(variable).c_str());
	}

	std::vector<int> FontMetafile::GetValuesOfVariable(const std::string &variable)
	{
		auto numbers = FormatString::Split(m_values.at(variable), NUMBER_SEPARATOR);
		auto result = std::vector<int>();

		int i = 0;

		for (auto number : numbers)
		{
			result.emplace_back(std::stoi(number));
			i++;
		}

		return result;
	}

	FontCharacter FontMetafile::GetCharacter(const int &ascii)
	{
		auto it = m_metadata.find(ascii);

		if (it != m_metadata.end())
		{
			return it->second;
		}

		return FontCharacter(-1, 0, 0, 0, 0, 0, 0, 0, 0, 0); // TODO: Use `std::optional`.
	}
}
