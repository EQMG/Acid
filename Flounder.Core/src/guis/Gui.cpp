#include "Gui.hpp"

namespace Flounder
{
	Gui::Gui(UiObject *parent, const Vector2 &position, const Vector2 &dimensions, Texture *texture, const int &selectedRow) :
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
		const int numberOfRows = m_texture != nullptr ? m_texture->GetNumberOfRows() : 1;
		const int column = m_selectedRow % numberOfRows;
		const int row = m_selectedRow / numberOfRows;
		m_textureOffset->Set(static_cast<float>(column / numberOfRows), static_cast<float>(row / numberOfRows));
	}
}
