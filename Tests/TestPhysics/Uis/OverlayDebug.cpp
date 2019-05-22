#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>
#include "World/World.hpp"

namespace test
{
OverlayDebug::OverlayDebug(UiObject *parent) :
	UiObject(parent, UiTransform(Vector2i(100, 48), UiAnchor::LeftBottom)),
	m_textFrameTime(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -2)), 11, "", FontType::Create("Fonts/ProximaNova", "Regular")),
	m_textFps(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -16)), 11, "", FontType::Create("Fonts/ProximaNova", "Regular")),
	m_textUps(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -30)), 11, "", FontType::Create("Fonts/ProximaNova", "Regular")),
	m_textTime(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -44)), 11, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left)
{
}

void OverlayDebug::UpdateObject()
{
	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));

	if (World::Get() != nullptr)
	{
		auto timePercent = (World::Get()->GetDayFactor() * 24.0f) + 6.0f;
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
