#include "guiobject.h"

namespace flounder
{
	guiobject::guiobject(screenobject *parent, const vector2 &position, const vector2 &dimensions, texture *texture, const int &selectedRow) :
		screenobject(parent, position, dimensions)
	{
		setMeshSize(vector2(0.5f, 0.5f));

		m_texture = texture;
		m_flipTexture = false;
		m_selectedRow = selectedRow;

		m_textureOffset = new vector2();
		m_colourOffset = new colour();
	}

	guiobject::~guiobject()
	{
		delete m_textureOffset;
		delete m_colourOffset;
	}

	void guiobject::updateObject()
	{
		int numberOfRows = m_texture != NULL ? m_texture->getNumberOfRows() : 1;
		int column = m_selectedRow % numberOfRows;
		int row = m_selectedRow / numberOfRows;
		m_textureOffset->set(column / numberOfRows, row / numberOfRows);
	}
}
