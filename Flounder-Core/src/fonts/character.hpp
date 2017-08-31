#pragma once

namespace Flounder
{
	/// <summary>
	/// Simple data structure class holding information about a certain glyph in the font texture atlas. All sizes are for a font-size of 1.
	/// </summary>
	class character
	{
	private:
		int m_id;
		double m_textureCoordX;
		double m_textureCoordY;
		double m_maxTextureCoordX;
		double m_maxTextureCoordY;
		double m_offsetX;
		double m_offsetY;
		double m_sizeX;
		double m_sizeY;
		double m_advanceX;
	public:
		/// <summary>
		/// Creates a new character.
		/// </summary>
		/// <param name="id"> The ASCII value of the character. </param>
		/// <param name="textureCoordX"> The x texture coordinate for the top left corner of the character in the texture atlas. </param>
		/// <param name="textureCoordY"> The y texture coordinate for the top left corner of the character in the texture atlas. </param>
		/// <param name="texureSizeX"> The width of the character in the texture atlas. </param>
		/// <param name="texureSizeY"> The height of the character in the texture atlas. </param>
		/// <param name="offsetX"> The x distance from the cursor to the left edge of the character's quad. </param>
		/// <param name="offsetY"> The y distance from the cursor to the top edge of the character's quad. </param>
		/// <param name="sizeX"> The width of the character's quad in screen space. </param>
		/// <param name="sizeY"> The height of the character's quad in screen space. </param>
		/// <param name="advanceX"> How far in pixels the cursor should advance after adding this character. </param>
		character(const int &id, const double &textureCoordX, const double &textureCoordY, const double &texureSizeX, const double &texureSizeY, const double &offsetX, const double &offsetY, const double &sizeX, const double &sizeY, const double &advanceX);

		/// <summary>
		/// Deconstructor for the character.
		/// </summary>
		~character();

		int getId() const { return m_id; }

		double getTextureCoordX() const { return m_textureCoordX; }

		double getTextureCoordY() const { return m_textureCoordY; }

		double getMaxTextureCoordX() const { return m_maxTextureCoordX; }

		double getMaxTextureCoordY() const { return m_maxTextureCoordY; }

		double getOffsetX() const { return m_offsetX; }

		double getOffsetY() const { return m_offsetY; }

		double getSizeX() const { return m_sizeX; }

		double getSizeY() const { return m_sizeY; }

		double getAdvanceX() const { return m_advanceX; }
	};
}
