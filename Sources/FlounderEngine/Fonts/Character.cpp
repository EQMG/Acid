#include "Character.hpp"

namespace Flounder
{
	Character::Character(const int &id, const double &textureCoordX, const double &textureCoordY, const double &textureSizeX, const double &textureSizeY, const double &offsetX, const double &offsetY, const double &sizeX, const double &sizeY, const double &advanceX) :
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

	Character::~Character()
	{
	}
}
