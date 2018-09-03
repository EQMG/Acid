#include "FontCharacter.hpp"

namespace acid
{
	FontCharacter::FontCharacter(const int &id, const float &textureCoordX, const float &textureCoordY, const float &textureSizeX, const float &textureSizeY, const float &offsetX, const float &offsetY, const float &sizeX, const float &sizeY, const float &advanceX) :
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

	FontCharacter::~FontCharacter()
	{
	}
}
