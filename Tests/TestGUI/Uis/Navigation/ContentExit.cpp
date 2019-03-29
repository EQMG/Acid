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
	m_iconify(&GetContent(), "Iconify", UiBound(Vector2(0.05f, 0.30f), UiReference::TopLeft)),
	m_antialiasing(&GetContent(), "Antialiasing", true, UiBound(Vector2(0.05f, 0.36f), UiReference::TopLeft)),
	m_borderless(&GetContent(), "Borderless", false, UiBound(Vector2(0.05f, 0.42f), UiReference::TopLeft)),
	m_resizable(&GetContent(), "Resizable", true, UiBound(Vector2(0.05f, 0.48f), UiReference::TopLeft)),
	m_floating(&GetContent(), "Floating", false, UiBound(Vector2(0.05f, 0.54f), UiReference::TopLeft)),
	m_fullscreen(&GetContent(), "Fullscreen", false, UiBound(Vector2(0.05f, 0.60f), UiReference::TopLeft)),
	m_fpsLimit(&GetContent(), "Fps Limit", 1000.0f, 0.0f, 1000.0f, 0, UiBound(Vector2(0.05f, 0.66f), UiReference::TopLeft))
{
	m_masterVolume.GetOnValue() += [this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Master, value / 100.0f);
	};
	m_generalVolume.GetOnValue() += [this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::General, value / 100.0f);
	};
	m_effectVolume.GetOnValue() += [this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Effect, value / 100.0f);
	};
	m_musicVolume.GetOnValue() += [this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Music, value / 100.0f);
	};
	Audio::Get()->GetOnGain() += [this](Audio::Type type, float volume)
	{
		switch (type)
		{
		case Audio::Type::Master:
			m_masterVolume.SetValue(volume * 100.0f);
			break;
		case Audio::Type::General:
			m_generalVolume.SetValue(volume * 100.0f);
			break;
		case Audio::Type::Effect:
			m_effectVolume.SetValue(volume * 100.0f);
			break;
		case Audio::Type::Music:
			m_musicVolume.SetValue(volume * 100.0f);
			break;
		}
	};

	m_iconify.GetOnClick() += [this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			Window::Get()->SetIconified(true);
		}
	};

	m_antialiasing.GetOnValue() += [this](bool value)
	{
	};

	m_borderless.GetOnValue() += [this](bool value)
	{
		Window::Get()->SetBorderless(value);
	};
	Window::Get()->GetOnBorderless() += [this](bool borderless)
	{
		m_borderless.SetValue(borderless);
	};

	m_resizable.GetOnValue() += [this](bool value)
	{
		Window::Get()->SetResizable(value);
	};
	Window::Get()->GetOnResizable() += [this](bool resizable)
	{
		m_resizable.SetValue(resizable);
	};

	m_floating.GetOnValue() += [this](bool value)
	{
		Window::Get()->SetFloating(value);
	};
	Window::Get()->GetOnFloating() += [this](bool floating)
	{
		m_floating.SetValue(floating);
	};

	m_fullscreen.GetOnValue() += [this](bool value)
	{
		Window::Get()->SetFullscreen(value);
	};
	Window::Get()->GetOnFullscreen() += [this](bool fullscreen)
	{
		m_fullscreen.SetValue(fullscreen);
	};

	m_fpsLimit.GetOnValue() += [this](float value)
	{
		Engine::Get()->SetFpsLimit(value);
	};
}

void ContentExit::UpdateObject()
{
	UiPanel::UpdateObject();
}
}