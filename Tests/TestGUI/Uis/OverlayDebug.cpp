#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_textFps(CreateStatus("FPS: 0", 0.002f, 0.978f, Text::Justify::Left)),
		m_textUps(CreateStatus("UPS: 0", 0.002f, 0.998f, Text::Justify::Left)),
		m_timerUpdate(Timer(Time::Seconds(0.333f)))
	{
	}

	void OverlayDebug::UpdateObject()
	{
		if (m_timerUpdate.IsPassedTime())
		{
			m_timerUpdate.ResetStartTime();

			m_textFps->SetString("FPS: " + String::To(Engine::Get()->GetFps()));
			m_textUps->SetString("UPS: " + String::To(Engine::Get()->GetUps()));
		}
	}

	std::unique_ptr<Text> OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Text::Justify &justify)
	{
		auto result = std::make_unique<Text>(this, UiBound(Vector2(positionX, positionY), UiReference::BottomLeft), 1.1f, content, FontType::Create("Fonts/ProximaNova", "Regular"), justify);
		result->SetTextColour(Colour::White);
		result->SetBorderColour(Colour(0.15f, 0.15f, 0.15f));
		result->SetBorderDriver<DriverConstant<float>>(0.04f);
		return result;
	}
}
