#include "UiTab.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include "UiNavigation.hpp"

namespace test
{
	UiTab::UiTab(UiObject *parent, UiObject *content, const UiBound &rectangle, const std::string &name, const Colour &colour) :
		UiObject(parent, rectangle),
		m_content(content),
		m_name(name),
		m_colour(colour),
		m_text(std::make_unique<Text>(this, rectangle, 1.6f, name, FontType::Resource("Fonts/ProximaNova", "Regular"), JUSTIFY_LEFT, 0.5f, 0.003f)),
		m_width(m_text->GetRectangle().m_dimensions.m_x * 1.6f),
		m_soundClick(Sound("Sounds/Button1.wav", 0.9f))
	{
		this->SetActionClick([&](MouseButton button) -> bool
		{
			if (button == MOUSE_BUTTON_LEFT)
			{
				dynamic_cast<UiNavigation *>(GetParent())->SwitchTab(m_name);
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
				return true;
			}

			return false;
		});
		m_content->SetAlphaDriver<DriverConstant>(0.0f);
	}

	void UiTab::UpdateObject()
	{
		GetRectangle().m_position.m_y = 1.0f;
		GetRectangle().m_dimensions = m_text->GetRectangle().m_dimensions;
		GetRectangle().m_dimensions.m_y = 0.125f;
	}
}
