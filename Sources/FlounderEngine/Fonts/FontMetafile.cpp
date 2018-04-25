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
		m_metadata(new std::map<int, FontCharacter *>()),
		m_values(new std::map<std::string, std::string>()),
		m_filename(filename),
		m_verticalPerPixelSize(0.0),
		m_horizontalPerPixelSize(0.0),
		m_imageWidth(0),
		m_spaceWidth(0.0),
		m_padding(new std::vector<int>()),
		m_paddingWidth(0),
		m_paddingHeight(0),
		m_maxSizeY(0.0)
	{
		std::string fileLoaded = FileSystem::ReadTextFile(filename);
		std::vector<std::string> lines = FormatString::Split(fileLoaded, "\n");

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
		delete m_metadata;
		delete m_values;

		delete m_padding;
	}

	void FontMetafile::ProcessNextLine(const std::string &line)
	{
		m_values->clear();
		std::vector<std::string> parts = FormatString::Split(line, SPLITTER);

		for (auto part : parts)
		{
			std::vector<std::string> pairs = FormatString::Split(part, "=");

			if (pairs.size() == 2)
			{
				m_values->insert(std::pair<std::string, std::string>(pairs.at(0), pairs.at(1)));
			}
		}
	}

	void FontMetafile::LoadPaddingData()
	{
		for (auto padding : GetValuesOfVariable("padding"))
		{
			m_padding->push_back(padding);
		}

		m_paddingWidth = m_padding->at(PAD_LEFT) + m_padding->at(PAD_RIGHT);
		m_paddingHeight = m_padding->at(PAD_TOP) + m_padding->at(PAD_BOTTOM);
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
		FontCharacter *c = LoadCharacter();

		if (c != nullptr)
		{
			m_metadata->insert(std::pair<int, FontCharacter *>(c->GetId(), c));
		}
	}

	FontCharacter *FontMetafile::LoadCharacter()
	{
		int id = GetValueOfVariable("id");

		if (id == SPACE_ASCII)
		{
			m_spaceWidth = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
			return nullptr;
		}

		double xTextureCoord = (static_cast<double>(GetValueOfVariable("x")) + (m_padding->at(PAD_LEFT) - DESIRED_PADDING)) / m_imageWidth;
		double yTextureCoord = (static_cast<double>(GetValueOfVariable("y")) + (m_padding->at(PAD_TOP) - DESIRED_PADDING)) / m_imageWidth;
		int width = GetValueOfVariable("width") - (m_paddingWidth - (2 * DESIRED_PADDING));
		int height = GetValueOfVariable("height") - ((m_paddingHeight) - (2 * DESIRED_PADDING));
		double quadWidth = width * m_horizontalPerPixelSize;
		double quadHeight = height * m_verticalPerPixelSize;
		double xTexSize = static_cast<double>(width) / m_imageWidth;
		double yTexSize = static_cast<double>(height) / m_imageWidth;
		double xOffset = (GetValueOfVariable("xoffset") + m_padding->at(PAD_LEFT) - DESIRED_PADDING) * m_horizontalPerPixelSize;
		double yOffset = (GetValueOfVariable("yoffset") + (m_padding->at(PAD_TOP) - DESIRED_PADDING)) * m_verticalPerPixelSize;
		double xAdvance = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;

		if (quadHeight > m_maxSizeY)
		{
			m_maxSizeY = quadHeight;
		}

		return new FontCharacter(id, xTextureCoord, yTextureCoord, xTexSize, yTexSize, xOffset, yOffset, quadWidth, quadHeight, xAdvance);
	}

	int FontMetafile::GetValueOfVariable(const std::string &variable)
	{
		return std::stoi(m_values->at(variable).c_str());
	}

	std::vector<int> FontMetafile::GetValuesOfVariable(const std::string &variable)
	{
		std::vector<std::string> numbers = FormatString::Split(m_values->at(variable), NUMBER_SEPARATOR);
		std::vector<int> result = std::vector<int>();

		int i = 0;

		for (const auto &number : numbers)
		{
			result.push_back(std::stoi(number));
			i++;
		}

		return result;
	}

	FontCharacter *FontMetafile::GetCharacter(const int &ascii)
	{
		auto it = m_metadata->find(ascii);

		if (it != m_metadata->end())
		{
			return it->second;
		}

		return nullptr;
	}
}
