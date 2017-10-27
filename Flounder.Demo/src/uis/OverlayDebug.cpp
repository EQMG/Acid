#include "OverlayDebug.hpp"

namespace Demo
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, Vector2(0.5f, 0.5f), Vector2(1.0f, 1.0f), Vector2(0.5f, 0.5f), true),
		//	m_textHeight(CreateStatus("HEIGHT: 0.0", 0.005f, 0.86f, AlignLeft)),
		//	m_textTime(CreateStatus("TIME: 0.0", 0.005f, 0.89f, AlignLeft)),
		//	m_textPosition(CreateStatus("POSITION: 0.0, 0.0, 0.0", 0.005f, 0.92f, AlignLeft)),
		//  m_textFps(CreateStatus("FPS: 0", 0.005f, 0.95f, AlignLeft)),
		//  m_textUps(CreateStatus("UPS: 0", 0.005f, 0.98f, AlignLeft)),
		m_guiExample(new Gui(this, Vector2(0.5f, 0.5f), Vector2(0.75f, 0.75f), Vector2(0.5f, 0.5f), true, new Texture("res/undefined.png"), 1)),
		m_timerUpdate(new Timer(0.333f))
	{
		m_guiExample->SetVisible(false);
	}

	OverlayDebug::~OverlayDebug()
	{
		//	delete m_textHeight;
		//	delete m_textTime;
		//	delete m_textPosition;
		//  delete m_textFps;
		//  delete m_textUps;
		delete m_guiExample;
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

			//m_textFps->SetText("FPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDeltaRender())));
			//m_textUps->SetText("UPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDelta())));
		}
	}

	Text *OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Justify &justify)
	{
		Text *result = new Text(this, Vector2(positionX, positionY), Vector2(0.5f, 0.5f), content, 1.0f, Uis::Get()->m_candara, 1.0f, justify);
		result->SetInScreenCoords(true);
		result->SetTextColour(Colour(1.0f, 1.0f, 1.0f));
		result->SetBorderColour(Colour(0.15f, 0.15f, 0.15f));
		result->SetBorder(new DriverConstant(0.04f));
		return result;
	}
}
