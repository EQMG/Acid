#pragma once

#include "Files/Files.hpp"

namespace acid
{
/**
 * @brief Provides functionality for getting the values from a font file.
 */
class FontMetafile
{
public:
	/**
	 * @brief Simple data structure class holding information about a certain glyph in the font texture atlas. All sizes are for a font-size of 1.
	 */
	class Character
	{
	public:
		/**
		 * Creates a new character.
		 * @param id The ASCII value of the character.
		 * @param textureCoordX The x texture coordinate for the top left corner of the character in the texture atlas.
		 * @param textureCoordY The y texture coordinate for the top left corner of the character in the texture atlas.
		 * @param textureSizeX The width of the character in the texture atlas.
		 * @param textureSizeY The height of the character in the texture atlas.
		 * @param offsetX The x distance from the cursor to the left edge of the character's quad.
		 * @param offsetY The y distance from the cursor to the top edge of the character's quad.
		 * @param sizeX The width of the character's quad in screen space.
		 * @param sizeY The height of the character's quad in screen space.
		 * @param advanceX How far in pixels the cursor should advance after adding this character.
		 */
		Character(const int32_t &id, const float &textureCoordX, const float &textureCoordY, const float &textureSizeX, const float &textureSizeY, const float &offsetX,
			const float &offsetY, const float &sizeX, const float &sizeY, const float &advanceX) :
			m_id(id),
			m_textureCoordX(textureCoordX),
			m_textureCoordY(textureCoordY),
			m_maxTextureCoordX(textureSizeX + textureCoordX),
			m_maxTextureCoordY(textureSizeY + textureCoordY),
			m_offsetX(offsetX),
			m_offsetY(offsetY),
			m_sizeX(sizeX),
			m_sizeY(sizeY),
			m_advanceX(advanceX)
		{
		}

		int32_t m_id;
		float m_textureCoordX;
		float m_textureCoordY;
		float m_maxTextureCoordX;
		float m_maxTextureCoordY;
		float m_offsetX;
		float m_offsetY;
		float m_sizeX;
		float m_sizeY;
		float m_advanceX;
	};

	static constexpr uint32_t PadTop = 0;
	static constexpr uint32_t PadLeft = 1;
	static constexpr uint32_t PadBottom = 2;
	static constexpr uint32_t PadRight = 3;
	static constexpr int32_t DesiredPassing = 8;

	static constexpr float LineHeight = 0.03f;
	static constexpr int32_t SpaceAscii = 32;

	/**
	 * Creates a new meta file.
	 * @param filename The font file to load from.
	 */
	explicit FontMetafile(std::string filename);

	std::optional<Character> GetCharacter(const int32_t &ascii) const;

	const std::string &GetFileName() const { return m_filename; }

	const float &GetSpaceWidth() const { return m_spaceWidth; }

	const float &GetMaxSizeY() const { return m_maxSizeY; }

private:
	/**
	 * Read in the next line and store the variable values.
	 * @param line The line to process.
	 */
	void ProcessNextLine(const std::string &line);

	/**
	 * Loads the data about how much padding is used around each character in the texture atlas.
	 */
	void LoadPaddingData();

	/**
	 * Loads information about the line height for this font in pixels,
	 * and uses this as a way to find the conversion rate between pixels in the texture atlas and screen-space.
	 */
	void LoadLineSizes();

	/**
	 * Loads in data about each character from the texture atlas and converts it all from 'pixels' to 'screen-space' before storing.
	 * And stores the data in the {@link Character} class. The effects of padding are also removed from the data.
	 */
	void LoadCharacterData();

	/**
	 * Gets the {@code int} value of the variable with a certain name on the current line.
	 * @tparam T The value type.
	 * @param variable The name of the variable.
	 * @return The value of the variable.
	 */
	template<typename T = int32_t>
	T GetValueOfVariable(const std::string &variable)
	{
		return String::From<T>(m_values.at(variable));
	}

	/**
	 * Gets the array of ints associated with a variable on the current line.
	 * @param variable The name of the variable.
	 * @return The {@code int} array of values associated with the variable.
	 */
	std::vector<int32_t> GetValuesOfVariable(const std::string &variable);

	std::map<int32_t, Character> m_characters;
	std::map<std::string, std::string> m_values;

	std::string m_filename;
	float m_verticalPerPixelSize;
	float m_horizontalPerPixelSize;
	int32_t m_imageWidth;
	float m_spaceWidth;
	std::vector<int32_t> m_padding;
	int32_t m_paddingWidth;
	int32_t m_paddingHeight;
	float m_maxSizeY;
};
}
