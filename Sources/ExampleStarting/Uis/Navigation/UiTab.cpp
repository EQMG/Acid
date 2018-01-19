#include "UiTab.hpp"

namespace Demo
{
	UiTab::UiTab(UiObject *parent, const UiBound &rectangle, const std::string &name, const Colour &colour) :
		UiObject(parent, rectangle),
		m_name(name),
		m_colour(colour),
		m_text(new Text(this, rectangle, 1.6f, name, Uis::Get()->m_proximaNova->GetRegular(), JustifyLeft, 1.0f, 0.003f)),
		m_width(m_text->GetRectangle()->m_dimensions->m_x * 1.6f)
	{
	}

	UiTab::~UiTab()
	{
		delete m_text;
	}

	void UiTab::UpdateObject()
	{
	}
}
