#include "Inspector.hpp"

#include <Audio/Audio.hpp>
#include <Devices/Keyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
Inspector::Inspector(UiObject *parent) :
	UiPanel(parent, UiTransform(Vector2f(1.0f, 0.0f), UiAnchor::TopRight, UiAspect::Position | UiAspect::Size, Vector2f(0.34f, 1.0f)), UiInputButton::BackgroundColour, Resize::Left,
		ScrollBar::Vertical),
	m_masterVolume(&GetContent(), "Master Volume", 100.0f, 0.0f, 100.0f, 0, UiTransform(Vector2f(0.92f, 0.06f), UiAnchor::TopRight)),
	m_dropdown(&GetContent(), "Dropdown", 0, { "A", "B", "C" }, UiTransform(Vector2f(0.92f, 0.12f), UiAnchor::TopRight)),
	m_generalVolume(&GetContent(), "General Volume", 100.0f, 0.0f, 100.0f, 0, UiTransform(Vector2f(0.92f, 0.18f), UiAnchor::TopRight)),
	m_effectVolume(&GetContent(), "Effect Volume", 100.0f, 0.0f, 100.0f, 0, UiTransform(Vector2f(0.92f, 0.24f), UiAnchor::TopRight)),
	m_musicVolume(&GetContent(), "Music Volume", 100.0f, 0.0f, 100.0f, 0, UiTransform(Vector2f(0.92f, 0.30f), UiAnchor::TopRight)),
	m_iconify(&GetContent(), "Iconify", UiTransform(Vector2f(0.92f, 0.36f), UiAnchor::TopRight)),
	m_antialiasing(&GetContent(), "Antialiasing", true, UiTransform(Vector2f(0.92f, 0.42f), UiAnchor::TopRight)),
	m_borderless(&GetContent(), "Borderless", false, UiTransform(Vector2f(0.92f, 0.48f), UiAnchor::TopRight)),
	m_resizable(&GetContent(), "Resizable", true, UiTransform(Vector2f(0.92f, 0.54f), UiAnchor::TopRight)),
	m_floating(&GetContent(), "Floating", false, UiTransform(Vector2f(0.92f, 0.60f), UiAnchor::TopRight)),
	m_fullscreen(&GetContent(), "Fullscreen", false, UiTransform(Vector2f(0.92f, 0.66f), UiAnchor::TopRight)),

	m_fpsLimit(&GetContent(), "Fps Limit", 1000.0f, 0.0f, 1000.0f, 0, UiTransform(Vector2f(0.92f, 0.72f), UiAnchor::TopRight)),
	m_textTitle(&GetContent(), "Title", Window::Get()->GetTitle(), 14, UiTransform(Vector2f(0.92f, 0.78f), UiAnchor::TopRight)),

	m_input1(&GetContent(), "Mouse Grabber", MouseButton::Left, UiTransform(Vector2f(0.92f, 0.84f), UiAnchor::TopRight)),
	m_input2(&GetContent(), "Keyboard Grabber", Key::W, UiTransform(Vector2f(0.92f, 0.90f), UiAnchor::TopRight)),
	m_input3(&GetContent(), "Joystick Grabber", 0, 3, UiTransform(Vector2f(0.92f, 0.96f), UiAnchor::TopRight)),
	m_radio1(&GetContent(), "First Value", UiInputRadio::Type::X, false, UiTransform(Vector2f(0.92f, 1.0f), UiAnchor::TopRight)),
	m_radio2(&GetContent(), "Second Value", UiInputRadio::Type::X, true, UiTransform(Vector2f(0.92f, 1.04f), UiAnchor::TopRight)),
	m_radio3(&GetContent(), "Third Value", UiInputRadio::Type::X, false, UiTransform(Vector2f(0.92f, 1.08f), UiAnchor::TopRight)),
	m_radioManager1(UiInputRadio::Type::Check, false, { &m_radio1, &m_radio2, &m_radio3 })
{
	m_masterVolume.OnValue().Add([this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Master, value / 100.0f);
	});
	m_generalVolume.OnValue().Add([this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::General, value / 100.0f);
	});
	m_effectVolume.OnValue().Add([this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Effect, value / 100.0f);
	});
	m_musicVolume.OnValue().Add([this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Music, value / 100.0f);
	});
	Audio::Get()->OnGain().Add([this](Audio::Type type, float volume)
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
	});

	m_iconify.OnClick().Add([this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			Window::Get()->SetIconified(true);
		}
	});

	m_antialiasing.OnValue().Add([this](bool value)
	{
	});

	m_borderless.OnValue().Add([this](bool value)
	{
		Window::Get()->SetBorderless(value);
	});
	Window::Get()->OnBorderless().Add([this](bool borderless)
	{
		m_borderless.SetValue(borderless);
	});

	m_resizable.OnValue().Add([this](bool value)
	{
		Window::Get()->SetResizable(value);
	});
	Window::Get()->OnResizable().Add([this](bool resizable)
	{
		m_resizable.SetValue(resizable);
	});

	m_floating.OnValue().Add([this](bool value)
	{
		Window::Get()->SetFloating(value);
	});
	Window::Get()->OnFloating().Add([this](bool floating)
	{
		m_floating.SetValue(floating);
	});

	m_fullscreen.OnValue().Add([this](bool value)
	{
		Window::Get()->SetFullscreen(value);
	});
	Window::Get()->OnFullscreen().Add([this](bool fullscreen)
	{
		m_fullscreen.SetValue(fullscreen);
	});

	m_fpsLimit.OnValue().Add([this](float value)
	{
		Engine::Get()->SetFpsLimit(value);
	});
	m_textTitle.OnValue().Add([this](std::string text)
	{
		Window::Get()->SetTitle(text);
	});
}

void Inspector::UpdateObject()
{
	UiPanel::UpdateObject();
}
}
