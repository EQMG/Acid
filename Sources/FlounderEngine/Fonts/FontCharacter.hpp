#pragma once

#include "Engine/Exports.hpp"

namespace fl
{
	/// <summary>
	/// Simple data structure class holding information about a certain glyph in the font texture atlas. All sizes are for a font-size of 1.
	/// </summary>
	class FL_EXPORT FontCharacter
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
		/// <param name="textureSizeX"> The width of the character in the texture atlas. </param>
		/// <param name="textureSizeY"> The height of the character in the texture atlas. </param>
		/// <param name="offsetX"> The x distance from the cursor to the left edge of the character's quad. </param>
		/// <param name="offsetY"> The y distance from the cursor to the top edge of the character's quad. </param>
		/// <param name="sizeX"> The width of the character's quad in screen space. </param>
		/// <param name="sizeY"> The height of the character's quad in screen space. </param>
		/// <param name="advanceX"> How far in pixels the cursor should advance after adding this character. </param>
		FontCharacter(const int &id, const double &textureCoordX, const double &textureCoordY, const double &textureSizeX, const double &textureSizeY, const double &offsetX, const double &offsetY, const double &sizeX, const double &sizeY, const double &advanceX);

		/// <summary>
		/// Deconstructor for the character.
		/// </summary>
		~FontCharacter();

		int GetId() const { return m_id; }

		double GetTextureCoordX() const { return m_textureCoordX; }

		double GetTextureCoordY() const { return m_textureCoordY; }

		double GetMaxTextureCoordX() const { return m_maxTextureCoordX; }

		double GetMaxTextureCoordY() const { return m_maxTextureCoordY; }

		double GetOffsetX() const { return m_offsetX; }

		double GetOffsetY() const { return m_offsetY; }

		double GetSizeX() const { return m_sizeX; }

		double GetSizeY() const { return m_sizeY; }

		double GetAdvanceX() const { return m_advanceX; }
	};
}
