#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>
#include "World/World.hpp"

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_textFrameTime(this, UiBound(Vector2(0.002f, 0.998f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
		m_textFps(this, UiBound(Vector2(0.002f, 0.978f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
		m_textUps(this, UiBound(Vector2(0.002f, 0.958f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
		m_textTime(this, UiBound(Vector2(0.002f, 0.938f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
		m_timerUpdate(Time::Seconds(0.5f))
	{
	}

	void OverlayDebug::UpdateObject()
	{
		m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
		m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
		m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));

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

				m_textTime.SetString("Time: " + String::To(hour) + ":" + String::To(minute));
			}
		}
	}
}
