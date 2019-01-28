#include "EditorPanels.hpp"

namespace test
{
	EditorPanels::EditorPanels(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_background(std::make_unique<Gui>(this, UiBound::Screen, Texture::Create("Guis/White.png"), Colour::White)),
		m_hierarchy(std::make_unique<Hierarchy>(this)),
		m_inspector(std::make_unique<Inspector>(this)),
		m_console(nullptr) // std::make_unique<Console>(this)
	{
	}

	void EditorPanels::UpdateObject()
	{
	}
}
