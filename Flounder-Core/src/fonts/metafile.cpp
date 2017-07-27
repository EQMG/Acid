#include "metafile.h"

namespace flounder
{
	const int metafile::PAD_TOP = 0;
	const int metafile::PAD_LEFT = 1;
	const int metafile::PAD_BOTTOM = 2;
	const int metafile::PAD_RIGHT = 3;
	const int metafile::DESIRED_PADDING = 8;

	const std::string metafile::SPLITTER = " ";
	const std::string metafile::NUMBER_SEPARATOR = ",";

	const double metafile::LINE_HEIGHT = 0.03f;
	const int metafile::NEWLINE_ASCII = 10;
	const int metafile::SPACE_ASCII = 32;

	metafile::metafile(const std::string &file)
	{
		m_metadata = new std::map<int, character*>();
		m_values = new std::map<std::string, std::string>();

		std::string fileLoaded = helperfile::readFile(file);
		std::vector<std::string> lines = helperstring::split(fileLoaded, "\n");

		for (std::string line : lines)
		{
			processNextLine(line);

			if (helperstring::contains(line, "info"))
			{
				loadPaddingData();
			}
			else if (helperstring::contains(line, "common"))
			{
				loadLineSizes();
			}
			else if (helperstring::contains(line, "char") && !helperstring::contains(line, "chars"))
			{
				loadCharacterData();
			}
		}
	}

	metafile::~metafile()
	{
		delete m_metadata;
		delete m_values;
	}

	void metafile::processNextLine(const std::string &line)
	{
		m_values->clear();
		std::vector<std::string> parts = helperstring::split(line, SPLITTER);

		for (std::string part : parts)
		{
			std::vector<std::string> pairs = helperstring::split(part, "=");

			if (pairs.size() == 2)
			{
				m_values->insert(std::pair<std::string, std::string>(pairs.at(0), pairs.at(1)));
			}
		}
	}

	void metafile::loadPaddingData()
	{
		m_padding = getValuesOfVariable("padding");
		m_paddingWidth = m_padding.at(PAD_LEFT) + m_padding.at(PAD_RIGHT);
		m_paddingHeight = m_padding.at(PAD_TOP) + m_padding.at(PAD_BOTTOM);
	}

	void metafile::loadLineSizes()
	{
		int lineHeightPixels = getValueOfVariable("lineHeight") - m_paddingHeight;
		m_verticalPerPixelSize = LINE_HEIGHT / static_cast<double>(lineHeightPixels);
		m_horizontalPerPixelSize = m_verticalPerPixelSize;
		m_imageWidth = getValueOfVariable("scaleW");
	}

	void metafile::loadCharacterData()
	{
		character *c = loadCharacter();

		if (c != NULL)
		{
			m_metadata->insert(std::pair<int, character*>(c->getId(), c));
		}
	}

	character *metafile::loadCharacter()
	{
		int id = getValueOfVariable("id");

		if (id == SPACE_ASCII)
		{
			m_spaceWidth = (getValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
			return NULL;
		}

		double xTextureCoord = (static_cast<double>(getValueOfVariable("x")) + (m_padding.at(PAD_LEFT) - DESIRED_PADDING)) / m_imageWidth;
		double yTextureCoord = (static_cast<double>(getValueOfVariable("y")) + (m_padding.at(PAD_TOP) - DESIRED_PADDING)) / m_imageWidth;
		int width = getValueOfVariable("width") - (m_paddingWidth - (2 * DESIRED_PADDING));
		int height = getValueOfVariable("height") - ((m_paddingHeight) - (2 * DESIRED_PADDING));
		double quadWidth = width * m_horizontalPerPixelSize;
		double quadHeight = height * m_verticalPerPixelSize;
		double xTexSize = static_cast<double>(width) / m_imageWidth;
		double yTexSize = static_cast<double>(height) / m_imageWidth;
		double xOffset = (getValueOfVariable("xoffset") + m_padding.at(PAD_LEFT) - DESIRED_PADDING) * m_horizontalPerPixelSize;
		double yOffset = (getValueOfVariable("yoffset") + (m_padding.at(PAD_TOP) - DESIRED_PADDING)) * m_verticalPerPixelSize;
		double xAdvance = (getValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
		
		if (quadHeight > m_maxSizeY)
		{
			m_maxSizeY = quadHeight;
		}

		return new character(id, xTextureCoord, yTextureCoord, xTexSize, yTexSize, xOffset, yOffset, quadWidth, quadHeight, xAdvance);
	}

	int metafile::getValueOfVariable(const std::string &variable)
	{
		return std::stoi(m_values->at(variable).c_str());
	}

	std::vector<int> metafile::getValuesOfVariable(const std::string &variable)
	{
		std::vector<std::string> numbers = helperstring::split(m_values->at(variable), NUMBER_SEPARATOR);
		std::vector<int> result = std::vector<int>();

		int i = 0;

		for (std::string number : numbers)
		{
			result.push_back(std::stoi(number.c_str()));
			i++;
		}

		return result;
	}
}
