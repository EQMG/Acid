#include "Console.hpp"

#include <Devices/Keyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
	Console::Console(UiObject *parent) :
		UiPanel(parent, UiBound(Vector2(0.4f, 1.0f), UiReference::BottomLeft, UiAspect::Dimensions, Vector2(0.8f, 0.3f)), ScrollBar::Vertical),
		m_text(std::make_unique<Text>(GetContent(), UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions),
			1.2f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.2f, Colour::White)),
		m_timerChange(Timer(Time::Seconds(0.8f)))
	{
		std::string sb = "";
		for (uint32_t i = 0 ; i < 15; i++)
		{
			sb += String::To(i);
			sb += "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
		}
		m_text->SetString(sb);
	}

	void Console::UpdateObject()
	{
		if (m_timerChange.IsPassedTime())
		{
			m_timerChange.ResetStartTime();

		//	m_text->SetString(Log::GetStream());
		}

		UiPanel::UpdateObject();
	}
}
