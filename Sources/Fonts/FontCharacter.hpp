#pragma once

#include <cstdint>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Simple data structure class holding information about a certain glyph in the font texture atlas. All sizes are for a font-size of 1.
	/// </summary>
	class ACID_EXPORT FontCharacter
	{
	private:
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
		FontCharacter(const int32_t &id, const float &textureCoordX, const float &textureCoordY, const float &textureSizeX, const float &textureSizeY, const float &offsetX, const float &offsetY, const float &sizeX, const float &sizeY, const float &advanceX);

		int32_t GetId() const { return m_id; }

		float GetTextureCoordX() const { return m_textureCoordX; }

		float GetTextureCoordY() const { return m_textureCoordY; }

		float GetMaxTextureCoordX() const { return m_maxTextureCoordX; }

		float GetMaxTextureCoordY() const { return m_maxTextureCoordY; }

		float GetOffsetX() const { return m_offsetX; }

		float GetOffsetY() const { return m_offsetY; }

		float GetSizeX() const { return m_sizeX; }

		float GetSizeY() const { return m_sizeY; }

		float GetAdvanceX() const { return m_advanceX; }
	};
}
