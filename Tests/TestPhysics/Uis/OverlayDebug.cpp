#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>
#include "World/World.hpp"

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_textFrameTime(CreateStatus("Frame Time: 0ms", 0.002f, 0.998f, Text::Justify::Left)),
		m_textFps(CreateStatus("FPS: 0", 0.002f, 0.978f, Text::Justify::Left)),
		m_textUps(CreateStatus("UPS: 0", 0.002f, 0.958f, Text::Justify::Left)),
		m_textTime(CreateStatus("Time: 0:00", 0.002f, 0.938f, Text::Justify::Left)),
		m_timerUpdate(Timer(Time::Seconds(0.5f)))
	{
	}

	void OverlayDebug::UpdateObject()
	{
		m_textFrameTime->SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
		m_textFps->SetString("FPS: " + String::To(Engine::Get()->GetFps()));
		m_textUps->SetString("UPS: " + String::To(Engine::Get()->GetUps()));

		if (m_timerUpdate.IsPassedTime())
		{
			m_timerUpdate.ResetStartTime();

			if (World::Get() != nullptr)
			{
				float timePercent = (World::Get()->GetDayFactor() * 24.0f) + 6.0f;
				auto hour = static_cast<int32_t>(timePercent);
				auto minute = static_cast<int32_t>((timePercent - hour) * 60.0f);

				if (hour > 24)
				{
					hour -= 24;
				}

				m_textTime->SetString("Time: " + String::To(hour) + ":" + String::To(minute));
			}
		}
	}

	std::unique_ptr<Text> OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Text::Justify &justify)
	{
		auto result = std::make_unique<Text>(this, UiBound(Vector2(positionX, positionY), UiReference::BottomLeft), 1.1f, content, FontType::Create("Fonts/ProximaNova", "Regular"), justify);
		result->SetTextColour(Colour("#ffffff"));
		result->SetBorderColour(Colour("#262626"));
		result->SetBorderDriver<DriverConstant<float>>(0.04f);
		return result;
	}
}
