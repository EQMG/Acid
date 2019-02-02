#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test
{
	OverlayDebug::OverlayDebug(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_textFps(CreateStatus("FPS: 0", 0.998f, 0.978f, Text::Justify::Left)),
		m_textUps(CreateStatus("UPS: 0", 0.998f, 0.998f, Text::Justify::Left)),
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
		}
	}

	std::unique_ptr<Text> OverlayDebug::CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Text::Justify &justify)
	{
		auto result = std::make_unique<Text>(this, UiBound(Vector2(positionX, positionY), UiReference::BottomRight), 1.1f, content, FontType::Create("Fonts/ProximaNova", "Regular"), justify);
		result->SetTextColour(Colour("#ffffff"));
		result->SetBorderColour(Colour("#262626"));
		result->SetBorderDriver<DriverConstant<float>>(0.04f);
		return result;
	}
}
