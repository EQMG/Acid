#include "character.hpp"

namespace flounder
{
	character::character(const int &id, const double &textureCoordX, const double &textureCoordY, const double &texureSizeX, const double &texureSizeY, const double &offsetX, const double &offsetY, const double &sizeX, const double &sizeY, const double &advanceX)
	{
		m_id = id;
		m_textureCoordX = textureCoordX;
		m_textureCoordY = textureCoordY;
		m_maxTextureCoordX = texureSizeX + textureCoordX;
		m_maxTextureCoordY = texureSizeY + textureCoordY;
		m_offsetX = offsetX;
		m_offsetY = offsetY;
		m_sizeX = sizeX;
		m_sizeY = sizeY;
		m_advanceX = advanceX;
	}

	character::~character()
	{
	}
}
