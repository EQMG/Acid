#include "UiTab.hpp"

#include <Visual/DriverConstant.hpp>
#include "UiNavigation.hpp"

namespace Demo
{
	UiTab::UiTab(UiObject *parent, UiObject *content, const UiBound &rectangle, const std::string &name, const Colour &colour) :
		UiObject(parent, rectangle),
		m_content(content),
		m_name(name),
		m_colour(colour),
		m_text(new Text(this, rectangle, 1.6f, name, Uis::Get()->m_proximaNova->GetRegular(), JustifyLeft, 0.5f, 0.003f)),
		m_width(m_text->GetRectangle()->m_dimensions->m_x * 1.6f)
	{
		this->SetActionLeft([&]()
		{
			dynamic_cast<UiNavigation *>(GetParent())->SwitchTab(m_name);
		});
		m_content->SetAlphaDriver(new DriverConstant(0.0f));
	}

	UiTab::~UiTab()
	{
		delete m_content;
		delete m_text;
	}

	void UiTab::UpdateObject()
	{
		GetRectangle()->m_position->m_y = 1.0f;
		GetRectangle()->m_dimensions->Set(*m_text->GetRectangle()->m_dimensions);
		GetRectangle()->m_dimensions->m_y = 0.125f;
	}
}
