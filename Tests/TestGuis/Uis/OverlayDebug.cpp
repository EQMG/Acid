#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_textInfo(CreateStatus("Build: d0.0.1", 0.002f, 0.002f, JUSTIFY_LEFT)),
		m_textFps(CreateStatus("FPS: 0", 0.002f, 0.042f, JUSTIFY_LEFT)),
		m_textUps(CreateStatus("UPS: 0", 0.002f, 0.062f, JUSTIFY_LEFT)),
		m_timerUpdate(Timer(0.333f))
	{
		//	m_textPosition->SetVisible(false);

	//	Gui *box = new Gui(Uis::Get()->GetContainer(), UiBound(Vector2(0.5f, 0.4f), "CentreRight", true, true, Vector2(0.6f, 0.6f)), Texture::Resource("Guis/Black.png"), 0);
	//	Text *title = new Text(box, UiBound(Vector2(0.5f, 0.9f), "TopCentre", true), 3.0f, "Hello World!", FontType::Resource("Fonts/ProximaNova", FAMILY_SEMIBOLD), JustifyLeft, 0.2f);
	//	title->SetTextColour(Colour::WHITE);
	}

	OverlayDebug::~OverlayDebug()
	{
		delete m_textInfo;
		delete m_textFps;
		delete m_textUps;
	}

	void OverlayDebug::UpdateObject()
	{
		if (m_timerUpdate.IsPassedTime())
		{
			m_timerUpdate.ResetStartTime();

			m_textFps->SetString("FPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDeltaRender())));
			m_textUps->SetString("UPS: " + std::to_string(static_cast<int>(1.0 / Engine::Get()->GetDelta())));
		}
	}

	Text *OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const FontJustify &justify)
	{
		Text *result = new Text(this, UiBound(Vector2(positionX, positionY), "BottomLeft", true), 1.1f, content, FontType::Resource("Fonts/ProximaNova", FAMILY_REGULAR), justify);
		result->SetTextColour(Colour(1.0f, 1.0f, 1.0f));
		result->SetBorderColour(Colour(0.15f, 0.15f, 0.15f));
		result->SetBorder(new DriverConstant(0.04f));
		return result;
	}
}
