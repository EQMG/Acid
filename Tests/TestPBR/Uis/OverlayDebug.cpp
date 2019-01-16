#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), UiBound::Centre, true, false, Vector2(1.0f, 1.0f))),
		m_textFps(CreateStatus("FPS: 0", 0.002f, 0.002f, Text::Justify::Left)),
		m_textUps(CreateStatus("UPS: 0", 0.002f, 0.022f, Text::Justify::Left)),
		m_timerUpdate(Timer(Time::Seconds(0.333f)))
	{
	}

	void OverlayDebug::UpdateObject()
	{
		if (m_timerUpdate.IsPassedTime())
		{
			m_timerUpdate.ResetStartTime();

			m_textFps->SetString("FPS: " + String::To(static_cast<int>(1.0f / Engine::Get()->GetDeltaRender().AsSeconds())));
			m_textUps->SetString("UPS: " + String::To(static_cast<int>(1.0f / Engine::Get()->GetDelta().AsSeconds())));
		}
	}

	std::unique_ptr<Text> OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Text::Justify &justify)
	{
		auto result = std::make_unique<Text>(this, UiBound(Vector2(positionX, positionY), UiBound::BottomLeft, true), 1.1f, content, FontType::Create("Fonts/ProximaNova", "Regular"), justify);
		result->SetTextColour(Colour("#ffffff"));
		result->SetBorderColour(Colour("#262626"));
		result->SetBorderDriver<DriverConstant>(0.04f);
		return result;
	}
}
