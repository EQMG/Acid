#include "OverlayDebug.hpp"

namespace Demo
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, Vector3(0.5f, 0.5f, true), Vector3(1.0f, 1.0f, true), Vector2(0.5f, 0.5f)),
		//	m_textHeight(CreateStatus("HEIGHT: 0.0", 0.005f, 0.86f, AlignLeft)),
		//	m_textTime(CreateStatus("TIME: 0.0", 0.005f, 0.89f, AlignLeft)),
		//	m_textPosition(CreateStatus("POSITION: 0.0, 0.0, 0.0", 0.005f, 0.92f, AlignLeft)),
		m_textFps(CreateStatus("FPS: 0", 0.005f, 0.95f, JustifyLeft)),
		m_textUps(CreateStatus("UPS: 0", 0.005f, 0.98f, JustifyLeft)),
		// m_textExample(new Text(this, Vector3(0.015f, 0.98f, false), Vector2(0.0f, 0.0f), "Kosmos", 1.25f, Uis::Get()->m_segoe, 0.7f, JustifyLeft)),
		m_textExample(new Text(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector2(0.0f, 0.0f), "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?", 1.2f, Uis::Get()->m_proximanova->GetRegular(), 0.75f, JustifyFully)),
		m_guiExample(new Gui(this, Vector3(0.5f, 1.0f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeNone), Vector2(0.5f, 0.0f), new Texture("res/guis/geometry-grain.jpg"), 1)), // 0.625f
		// m_guiExample(new Gui(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(0.5f, 0.5f, RelativeScreen), Vector2(0.5f, 0.0f), new Texture("res/guis/geometry-grain.jpg"), 1)),
		m_timerUpdate(new Timer(0.333f))
	{
#ifndef FLOUNDER_CONFIG_RELEASE
		m_textExample->SetTextColour(Colour("#ffffff"));

		/*Texture *textureInv = new Texture("res/guis/geometry-grain.jpg");

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				const float size = 0.1f;
				float x = i * (size + 0.025f);
				float y = j * (size + 0.025f);
				//Gui *gui = new Gui(this, Vector3(0.05f + x, 0.175f + y, false), Vector3(size, size, true), Vector2(0.0f, 0.0f), textureInv, 1);
				Gui *gui = new Gui(this, Vector3(0.5f, 0.5f, true), Vector3(1.0f, 1.0f, false), Vector2(0.5f, 0.5f), textureInv, 1);
				gui->SetScissor(Vector4(x + 0.05f, y - 0.825f, size, size));
				gui->SetColourOffset(Colour("#D14841"));
			}
		}*/

		m_guiExample->SetScissor(Vector4(0.0f, 0.875f, 1.0f, 1.0f));
		m_guiExample->SetColourOffset(Colour("#553982"));
#else
		m_textExample->SetVisible(false);
		m_guiExample->SetVisible(false);
#endif
	}

	OverlayDebug::~OverlayDebug()
	{
		//	delete m_textHeight;
		//	delete m_textTime;
		//	delete m_textPosition;
		delete m_textFps;
		delete m_textUps;
		delete m_textExample;
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

			m_textFps->SetText("FPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDeltaRender())));
			m_textUps->SetText("UPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDelta())));
		}
	}

	Text *OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Justify &justify)
	{
		Text *result = new Text(this, Vector3(positionX, positionY, true), Vector2(0.5f, 0.5f), content, 1.0f, Uis::Get()->m_candara->GetRegular(), 1.0f, justify);
		result->SetTextColour(Colour(1.0f, 1.0f, 1.0f));
		result->SetBorderColour(Colour(0.15f, 0.15f, 0.15f));
		result->SetBorder(new DriverConstant(0.04f));
		return result;
	}
}
