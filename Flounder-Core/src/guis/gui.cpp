#include "gui.h"

namespace flounder
{
	gui::gui(uiobject *parent, const vector2 &position, const vector2 &dimensions, texture *texture, const int &selectedRow) :
		uiobject(parent, position, dimensions)
	{
		setMeshSize(vector2(0.5f, 0.5f));

		m_texture = texture;
		m_flipTexture = false;
		m_selectedRow = selectedRow;

		m_textureOffset = new vector2();
		m_colourOffset = new colour();
	}

	gui::~gui()
	{
		delete m_textureOffset;
		delete m_colourOffset;
	}

	void gui::updateObject()
	{
		int numberOfRows = m_texture != NULL ? m_texture->getNumberOfRows() : 1;
		int column = m_selectedRow % numberOfRows;
		int row = m_selectedRow / numberOfRows;
		m_textureOffset->set(static_cast<float>(column / numberOfRows), static_cast<float>(row / numberOfRows));
	}
}
