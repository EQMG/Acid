#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>
#include "World/World.hpp"

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), UiBound::Centre, true, false, Vector2(1.0f, 1.0f))),
		m_textFps(CreateStatus("FPS: 0", 0.002f, 0.002f, Text::Justify::Left)),
		m_textUps(CreateStatus("UPS: 0", 0.002f, 0.022f, Text::Justify::Left)),
		m_textPosition(CreateStatus("Pos: 0.0, 0.0, 0.0", 0.002f, 0.042f, Text::Justify::Left)),
		m_textTime(CreateStatus("Time: 0:00", 0.002f, 0.062f, Text::Justify::Left)),
		m_timerUpdate(Timer(Time::Seconds(0.5f))),
		m_accumulatedFps(0.0f),
		m_accumulatedUps(0.0f),
		m_ticksPassed(0)
	{
	}

	void OverlayDebug::UpdateObject()
	{
		m_accumulatedFps += 1.0f / Engine::Get()->GetDeltaRender().AsSeconds();
		m_accumulatedUps += 1.0f / Engine::Get()->GetDelta().AsSeconds();
		m_ticksPassed++;

		if (m_timerUpdate.IsPassedTime())
		{
			m_timerUpdate.ResetStartTime();

			float fps = m_accumulatedFps / static_cast<float>(m_ticksPassed);
			float ups = m_accumulatedUps / static_cast<float>(m_ticksPassed);
			m_accumulatedFps = 0.0f;
			m_accumulatedUps = 0.0f;
			m_ticksPassed = 0;

			m_textFps->SetString("FPS: " + String::To(static_cast<int>(fps)));
			m_textUps->SetString("UPS: " + String::To(static_cast<int>(ups)));

			if (Scenes::Get()->GetCamera() != nullptr)
			{
				Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
				m_textPosition->SetString("Pos: " + String::To(static_cast<int>(cameraPosition.m_x)) + ", " +
					String::To(static_cast<int>(cameraPosition.m_y)) + ", " +
					String::To(static_cast<int>(cameraPosition.m_z)));
			}

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
		auto result = std::make_unique<Text>(this, UiBound(Vector2(positionX, positionY), UiBound::BottomLeft, true), 1.1f, content, FontType::Create("Fonts/ProximaNova", "Regular"), justify);
		result->SetTextColour(Colour("#ffffff"));
		result->SetBorderColour(Colour("#262626"));
		result->SetBorderDriver<DriverConstant>(0.04f);
		return result;
	}
}
