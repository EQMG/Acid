#include "Gui.hpp"

namespace Flounder
{
	Gui::Gui(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, texture *texture, const int &selectedRow) :
		UiObject(parent, position, dimensions),
		m_texture(texture),
		m_flipTexture(false),
		m_selectedRow(selectedRow),
		m_textureOffset(new Vector2()),
		m_colourOffset(new Colour())
	{
		SetMeshSize(Vector2(0.5f, 0.5f));
	}

	Gui::~Gui()
	{
		delete m_textureOffset;
		delete m_colourOffset;
	}

	void Gui::UpdateObject()
	{
		int numberOfRows = m_texture != nullptr ? m_texture->getNumberOfRows() : 1;
		int column = m_selectedRow % numberOfRows;
		int row = m_selectedRow / numberOfRows;
		m_textureOffset->Set(static_cast<float>(column / numberOfRows), static_cast<float>(row / numberOfRows));
	}
}
