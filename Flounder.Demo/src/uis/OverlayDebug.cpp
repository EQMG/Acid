#include "OverlayDebug.hpp"

#include <uis/UiBound.hpp>

namespace Demo
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), Vector2(0.5f, 0.5f), true, true, Vector2(1.0f, 1.0f))),
		//	m_textHeight(CreateStatus("HEIGHT: 0.0", 0.005f, 0.15f, JustifyLeft)),
		//	m_textTime(CreateStatus("TIME: 0.0", 0.005f, 0.12f, JustifyLeft)),
		//	m_textPosition(CreateStatus("POSITION: 0.0, 0.0, 0.0", 0.005f, 0.09f, JustifyLeft)),
		m_textFps(CreateStatus("FPS: 0", 0.005f, 0.06f, JustifyLeft)),
		m_textUps(CreateStatus("UPS: 0", 0.005f, 0.03f, JustifyLeft)),
		m_timerUpdate(new Timer(0.333f))
	{
	//	m_textFps->SetVisible(false);
	//	m_textUps->SetVisible(false);
	}

	OverlayDebug::~OverlayDebug()
	{
		//	delete m_textHeight;
		//	delete m_textTime;
		//	delete m_textPosition;
		delete m_textFps;
		delete m_textUps;
		delete m_timerUpdate;
	}

	void OverlayDebug::UpdateObject()
	{
		if (m_timerUpdate->IsPassedTime())
		{
			m_timerUpdate->ResetStartTime();

			//if (Terrains::Get() != nullptr)
			//{
			//	float terrainHeight = 0.0f; //  terrains::get()->getTerrain()->getHeight(position->m_x, position->m_z);
			//	m_textHeight->SetText("HEIGHT: " + std::to_string(Maths::RoundToPlace(terrainHeight, 2)));
			//}

			//if (Worlds::Get() != nullptr)
			//{
			//	m_textTime->SetText("TIME: " + std::to_string(static_cast<int>(Worlds::Get()->GetDayFactor() * 100.0f)) + "%");
			//}

			//if (Camera::Get() != nullptr)
			//{
			//	Vector3 *position = Camera::Get()->GetCamera()->GetPosition();
			//	m_textPosition->SetText("POSITION: " + std::to_string(static_cast<int>(position->m_x)) + ", " + std::to_string(static_cast<int>(position->m_y)) + ", " + std::to_string(static_cast<int>(position->m_z)));
			//}

			m_textFps->SetText("FPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDeltaRender())));
			m_textUps->SetText("UPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDelta())));
		}
	}

	Text *OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Justify &justify)
	{
		Text *result = new Text(this, UiBound(Vector2(positionX, positionY), Vector2(0.0f, 1.0f), true), 1.4f, content, Uis::Get()->m_candara->GetRegular(), justify);
		result->SetTextColour(Colour(1.0f, 1.0f, 1.0f));
		result->SetBorderColour(Colour(0.15f, 0.15f, 0.15f));
		result->SetBorder(new DriverConstant(0.04f));
		return result;
	}
}
