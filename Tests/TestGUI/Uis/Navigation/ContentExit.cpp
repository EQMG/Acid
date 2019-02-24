#include "ContentExit.hpp"

#include "UiNavigation.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiPanel(parent, UiBound(Vector2(0.45f, 0.5f), UiReference::CentreLeft, UiAspect::Dimensions, Vector2(0.8f, 0.8f)), ScrollBar::None),
		m_masterVolume(&GetContent(), "Master Volume", 100.0f, 0.0f, 100.0f, 0, UiBound(Vector2(0.05f, 0.06f), UiReference::TopLeft)),
		m_generalVolume(&GetContent(), "General Volume", 100.0f, 0.0f, 100.0f, 0, UiBound(Vector2(0.05f, 0.12f), UiReference::TopLeft)),
		m_effectVolume(&GetContent(), "Effect Volume", 100.0f, 0.0f, 100.0f, 0, UiBound(Vector2(0.05f, 0.18f), UiReference::TopLeft)),
		m_musicVolume(&GetContent(), "Music Volume", 100.0f, 0.0f, 100.0f, 0, UiBound(Vector2(0.05f, 0.24f), UiReference::TopLeft)),
		m_antialiasing(&GetContent(), "Antialiasing", false, UiBound(Vector2(0.05f, 0.30f), UiReference::TopLeft)),
		m_fpsLimit(&GetContent(), "Fps Limit", 1000.0f, 0.0f, 1000.0f, 0, UiBound(Vector2(0.05f, 0.36f), UiReference::TopLeft))
	{
		m_masterVolume.GetOnSlide() += [this](UiInputSlider *object, float value)
		{
			Audio::Get()->SetMasterGain(value / 100.0f);
		};
		m_generalVolume.GetOnSlide() += [this](UiInputSlider *object, float value)
		{
			Audio::Get()->SetTypeGain(Audio::Type::General, value / 100.0f);
		};
		m_effectVolume.GetOnSlide() += [this](UiInputSlider *object, float value)
		{
			Audio::Get()->SetTypeGain(Audio::Type::Effect, value / 100.0f);
		};
		m_musicVolume.GetOnSlide() += [this](UiInputSlider *object, float value)
		{
			Audio::Get()->SetTypeGain(Audio::Type::Music, value / 100.0f);
		};
		m_fpsLimit.GetOnSlide() += [this](UiInputSlider *object, float value)
		{
			Engine::Get()->SetFpsLimit(value);
		};
	}

	void ContentExit::UpdateObject()
	{
		UiPanel::UpdateObject();
	}
}