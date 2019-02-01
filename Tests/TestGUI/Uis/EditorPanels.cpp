#include "EditorPanels.hpp"

#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
	EditorPanels::EditorPanels(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_background(std::make_unique<Gui>(this, UiBound::Screen, Texture::Create("Guis/White.png"), Colour::White)),
		m_hierarchy(std::make_unique<Hierarchy>(this)),
		m_inspector(std::make_unique<Inspector>(this)),
		m_console(nullptr), // std::make_unique<Console>(this)
		m_banner(std::make_unique<Gui>(this, UiBound(Vector2(0.5f, 0.5f), UiReference::Centre), Texture::Create("Guis/Banner.png"), Colour::White))
	{
		m_banner->SetNinePatches(Vector4(0.195f, 0.0f, 0.847f, 0.06f));
		m_banner->SetHeight(1.0f);
	}

	void EditorPanels::UpdateObject()
	{
	//	m_banner->SetEnabled(true);
	//	m_banner->GetRectangle().SetDimensions(-2.0f * (0.5f - Mouse::Get()->GetPosition()) * Vector2(Window::Get()->GetAspectRatio(), 1.0f));
	}
}
