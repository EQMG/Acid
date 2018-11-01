#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_textFps(CreateStatus("FPS: 0", 0.002f, 0.002f, TEXT_JUSTIFY_LEFT)),
		m_textUps(CreateStatus("UPS: 0", 0.002f, 0.022f, TEXT_JUSTIFY_LEFT)),
		m_levelIcons(std::vector<std::unique_ptr<Gui>>()),
		m_levelTexts(std::vector<std::unique_ptr<Text>>()),
		m_timerUpdate(Timer(Time::Seconds(0.333f)))
	{
		for (uint32_t i = 0; i < 5; i++)
		{
			auto levelIcon = std::make_unique<Gui>(this, UiBound(Vector2(0.11f + (i * 0.064f), 0.01f), "BottomLeft", false, true, Vector2(0.08f, 0.08f)), Texture::Resource("Guis/Level_1x1.png"));
			auto levelText = std::make_unique<Text>(levelIcon.get(), UiBound(Vector2(0.1138f + (i * 0.064f), 0.01f), "BottomLeft", false), 4.2f, String::To(i + 1), FontType::Resource("Fonts/ProximaNova", "Bold"), TEXT_JUSTIFY_LEFT, 0.08f);
			levelText->SetTextColour(Colour("#E4E7B8"));
			m_levelIcons.emplace_back(std::move(levelIcon));
			m_levelTexts.emplace_back(std::move(levelText));
		}

		m_levelTexts[0]->SetTextColour(Colour("#E6C459"));
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

	std::unique_ptr<Text> OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const TextJustify &justify)
	{
		auto result = std::make_unique<Text>(this, UiBound(Vector2(positionX, positionY), "BottomLeft", true), 1.1f, content, FontType::Resource("Fonts/ProximaNova", "Regular"), justify);
		result->SetTextColour(Colour("#ffffff"));
		result->SetBorderColour(Colour("#262626"));
		result->SetBorderDriver<DriverConstant>(0.04f);
		return result;
	}
}
