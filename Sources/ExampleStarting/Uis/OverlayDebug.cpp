#include "OverlayDebug.hpp"

#include <Visual/DriverConstant.hpp>
#include <Worlds/Worlds.hpp>
#include <Camera/Camera.hpp>

namespace Demo
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_textInfo(CreateStatus("Build: d0.0.1", 0.002f, 0.002f, JustifyLeft)),
		m_textTime(CreateStatus("Time: 0:00", 0.002f, 0.022f, JustifyLeft)),
		m_textFps(CreateStatus("FPS: 0", 0.002f, 0.042f, JustifyLeft)),
		m_textUps(CreateStatus("UPS: 0", 0.002f, 0.062f, JustifyLeft)),
		m_textPosition(CreateStatus("POSITION: 0.0, 0.0, 0.0", 0.002f, 0.082f, JustifyLeft)),
		m_timerUpdate(new Timer(0.333f))
	{
		//	m_textPosition->SetVisible(false);
	}

	OverlayDebug::~OverlayDebug()
	{
		delete m_textInfo;
		delete m_textTime;
		delete m_textFps;
		delete m_textUps;
		delete m_textPosition;
		delete m_timerUpdate;
	}

	void OverlayDebug::UpdateObject()
	{
		if (m_timerUpdate->IsPassedTime())
		{
			m_timerUpdate->ResetStartTime();

			if (Worlds::Get() != nullptr)
			{
				float timePercent = (Worlds::Get()->GetDayFactor() * 24.0f) + 6.0f;
				int hour = static_cast<int>(timePercent);
				int minute = static_cast<int>((timePercent - hour) * 60.0f);

				if (hour > 24)
				{
					hour -= 24;
				}

				m_textTime->SetText("Time: " + std::to_string(hour) + ":" + std::to_string(minute));
			}

			if (Camera::Get() != nullptr)
			{
				Vector3 *position = Camera::Get()->GetCamera()->GetPosition();
				m_textPosition->SetText("POS: " + std::to_string(static_cast<int>(position->m_x)) + ", " +
					std::to_string(static_cast<int>(position->m_y)) + ", " +
					std::to_string(static_cast<int>(position->m_z)));
			}

			m_textFps->SetText("FPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDeltaRender())));
			m_textUps->SetText("UPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDelta())));
		}
	}

	Text *OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Justify &justify)
	{
		Text *result = new Text(this, UiBound(Vector2(positionX, positionY), "BottomLeft", true), 1.1f, content, Uis::Get()->m_proximaNova->GetRegular(), justify);
		result->SetTextColour(Colour(1.0f, 1.0f, 1.0f));
		result->SetBorderColour(Colour(0.15f, 0.15f, 0.15f));
		result->SetBorder(new DriverConstant(0.04f));
		return result;
	}
}
