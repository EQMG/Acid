#include "FontMetafile.hpp"

#include "Helpers/FileSystem.hpp"
#include "Helpers/String.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
	const uint32_t FontMetafile::PAD_TOP = 0;
	const uint32_t FontMetafile::PAD_LEFT = 1;
	const uint32_t FontMetafile::PAD_BOTTOM = 2;
	const uint32_t FontMetafile::PAD_RIGHT = 3;
	const int32_t FontMetafile::DESIRED_PADDING = 8;

	const std::string FontMetafile::SPLITTER = " ";
	const std::string FontMetafile::NUMBER_SEPARATOR = ",";

	const float FontMetafile::LINE_HEIGHT = 0.03f;
	const int32_t FontMetafile::SPACE_ASCII = 32;

	std::shared_ptr<FontMetafile> FontMetafile::Resource(const std::string &filename)
	{
		if (filename.empty())
		{
			return nullptr;
		}

		auto resource = Resources::Get()->Find(filename);

		if (resource != nullptr)
		{
			return std::dynamic_pointer_cast<FontMetafile>(resource);
		}

		auto result = std::make_shared<FontMetafile>(filename);
		Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
		return result;
	}

	FontMetafile::FontMetafile(const std::string &filename) :
		m_metadata(std::map<int32_t, FontCharacter>()),
		m_values(std::map<std::string, std::string>()),
		m_filename(filename),
		m_verticalPerPixelSize(0.0f),
		m_horizontalPerPixelSize(0.0f),
		m_imageWidth(0),
		m_spaceWidth(0.0f),
		m_padding(std::vector<int32_t>()),
		m_paddingWidth(0),
		m_paddingHeight(0),
		m_maxSizeY(0.0f)
	{
		auto fileLoaded = Files::Read(m_filename);

		if (!fileLoaded)
		{
			Log::Error("Font Metafile could not be loaded: '%s'\n", m_filename.c_str());
			return;
		}

		auto lines = String::Split(*fileLoaded, "\n");

		for (auto &line : lines)
		{
			ProcessNextLine(line);

			if (String::Contains(line, "info"))
			{
				LoadPaddingData();
			}
			else if (String::Contains(line, "common"))
			{
				LoadLineSizes();
			}
			else if (String::Contains(line, "char") && !String::Contains(line, "chars"))
			{
				LoadCharacterData();
			}
		}
	}

	std::optional<FontCharacter> FontMetafile::GetCharacter(const int32_t &ascii)
	{
		auto it = m_metadata.find(ascii);

		if (it != m_metadata.end())
		{
			return it->second;
		}

		return {};
	}

	void FontMetafile::ProcessNextLine(const std::string &line)
	{
		m_values.clear();
		auto parts = String::Split(line, SPLITTER);

		for (auto &part : parts)
		{
			auto pairs = String::Split(part, "=");

			if (pairs.size() == 2)
			{
				m_values.emplace(pairs.at(0), pairs.at(1));
			}
		}
	}

	void FontMetafile::LoadPaddingData()
	{
		for (auto &padding : GetValuesOfVariable("padding"))
		{
			m_padding.emplace_back(padding);
		}

		m_paddingWidth = m_padding.at(PAD_LEFT) + m_padding.at(PAD_RIGHT);
		m_paddingHeight = m_padding.at(PAD_TOP) + m_padding.at(PAD_BOTTOM);
	}

	void FontMetafile::LoadLineSizes()
	{
		int32_t lineHeightPixels = GetValueOfVariable("lineHeight") - m_paddingHeight;
		m_verticalPerPixelSize = LINE_HEIGHT / static_cast<float>(lineHeightPixels);
		m_horizontalPerPixelSize = m_verticalPerPixelSize;
		m_imageWidth = GetValueOfVariable("scaleW");
	}

	void FontMetafile::LoadCharacterData()
	{
		int32_t id = GetValueOfVariable("id");

		if (id == SPACE_ASCII)
		{
			m_spaceWidth = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;
			return;
		}

		float xTextureCoord = (static_cast<float>(GetValueOfVariable("x")) + (m_padding.at(PAD_LEFT) - DESIRED_PADDING)) / m_imageWidth;
		float yTextureCoord = (static_cast<float>(GetValueOfVariable("y")) + (m_padding.at(PAD_TOP) - DESIRED_PADDING)) / m_imageWidth;
		int32_t width = GetValueOfVariable("width") - (m_paddingWidth - (2 * DESIRED_PADDING));
		int32_t height = GetValueOfVariable("height") - ((m_paddingHeight) - (2 * DESIRED_PADDING));
		float quadWidth = width * m_horizontalPerPixelSize;
		float quadHeight = height * m_verticalPerPixelSize;
		float xTexSize = static_cast<float>(width) / m_imageWidth;
		float yTexSize = static_cast<float>(height) / m_imageWidth;
		float xOffset = (GetValueOfVariable("xoffset") + m_padding.at(PAD_LEFT) - DESIRED_PADDING) * m_horizontalPerPixelSize;
		float yOffset = (GetValueOfVariable("yoffset") + (m_padding.at(PAD_TOP) - DESIRED_PADDING)) * m_verticalPerPixelSize;
		float xAdvance = (GetValueOfVariable("xadvance") - m_paddingWidth) * m_horizontalPerPixelSize;

		if (quadHeight > m_maxSizeY)
		{
			m_maxSizeY = quadHeight;
		}

		FontCharacter character = FontCharacter(id, xTextureCoord, yTextureCoord, xTexSize, yTexSize, xOffset, yOffset, quadWidth, quadHeight, xAdvance);
		m_metadata.emplace(character.GetId(), character);
	}

	int32_t FontMetafile::GetValueOfVariable(const std::string &variable)
	{
		return String::From<int32_t>(m_values.at(variable));
	}

	std::vector<int32_t> FontMetafile::GetValuesOfVariable(const std::string &variable)
	{
		auto result = std::vector<int32_t>();
		auto numbers = String::Split(m_values.at(variable), NUMBER_SEPARATOR);

		for (auto &number : numbers)
		{
			result.emplace_back(String::From<int32_t>(number));
		}

		return result;
	}
}
