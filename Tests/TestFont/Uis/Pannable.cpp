﻿#include "Pannable.hpp"

#include <Audio/Audio.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Uis/Inputs/UiInputButton.hpp>
#include <Graphics/Graphics.hpp>
#include <Uis/Uis.hpp>

namespace test
{
	Pannable::Pannable(UiObject *parent) :
		UiObject(parent, UiTransform(Window::Get()->GetSize())),
		m_buttonReset({ Key::Enter }),
		m_testCompound(ButtonCompound::Create<ButtonKeyboard>(true, Key::G, Key::H, Key::J)),
		m_testHat(0, 0, JoystickHat::Up | JoystickHat::Right),
		m_settings(parent, UiTransform(Vector2i(300, 300), UiAnchor::LeftTop, Vector2i(20, 20)), UiInputButton::BackgroundColour, UiManipulate::All,
			ScrollBar::None),
		m_masterVolume(&m_settings.GetContent(), "Master Volume", 100.0f, 0.0f, 100.0f, 0, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, 0))),
		m_antialiasing(&m_settings.GetContent(), "Antialiasing", true, UiTransform(UiInputButton::Size, UiAnchor::LeftTop, Vector2i(0, 28))),
		m_zoom(1.0f),
		m_title(this, UiTransform(Vector2i(300, 80), UiAnchor::CentreTop), 72, "Acid Font",
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, Colour::Red),
		m_body(this, UiTransform(Vector2i(500, 1000), UiAnchor::CentreTop, Vector2i(0, 100)), 12, "",
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, Colour::Black),
		m_textFrameTime(parent, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -2)), 11, "Frame Time: 0ms", FontType::Create("Fonts/ProximaNova", "Regular"),
			Text::Justify::Left),
		m_textFps(parent, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -16)), 11, "FPS: 0", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left),
		m_textUps(parent, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -30)), 11, "UPS: 0", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left)
{
	m_buttonReset.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			m_zoom = 1.0f;
			GetTransform().SetPosition(Vector2f(0.5f, 0.5f));
		}
		Log::Out("Button Reset: %i\n", action);
	});
	m_testCompound->OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		Log::Out("Test Compound: %i\n", action);
	});
	m_testHat.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		Log::Out("Test Hat: %i\n", action);
	});

	m_settings.GetTransform().SetDepth(-4.0f);
	m_masterVolume.OnValue().Add([this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Master, value / 100.0f);
	});
	m_antialiasing.OnValue().Add([this](bool value)
	{
	});

	m_body.SetString(//L"Hello world, Привет мир, schön! 0123456789 #$%^*@&( []{} «»½¼±¶§\n"
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus sit amet scelerisque augue, sit amet commodo neque. Vestibulum\n"
		"eu eros a justo molestie bibendum quis in urna. Integer quis tristique magna. Morbi in ultricies lorem. Donec lacinia nisi et\n"
		"arcu scelerisque, eget viverra ante dapibus. Proin enim neque, vehicula id congue quis, consequat sit amet tortor.Aenean ac\n"
		"lorem sit amet magna rhoncus rhoncus ac ac neque. Cras sed rutrum sem. Donec placerat ultricies ex, a gravida lorem commodo ut.\n"
		"Mauris faucibus aliquet ligula, vitae condimentum dui semper et. Aenean pellentesque ac ligula a varius. Suspendisse congue\n"
		"lorem lorem, ac consectetur ipsum condimentum id.\n\n"
		"Vestibulum quis erat sem. Fusce efficitur libero et leo sagittis, ac volutpat felis ullamcorper. Curabitur fringilla eros eget ex\n"
		"lobortis, at posuere sem consectetur. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis\n"
		"egestas. Vivamus eu enim leo. Morbi ultricies lorem et pellentesque vestibulum. Proin eu ultricies sem. Quisque laoreet, ligula\n"
		"non molestie congue, odio nunc tempus arcu, vel aliquet leo turpis non enim. Sed risus dui, condimentum et eros a, molestie\n"
		"imperdiet nisl. Vivamus quis ante venenatis, cursus magna ut, tincidunt elit. Aenean nisl risus, porttitor et viverra quis,\n"
		"tempus vitae nisl.\n\n"
		"Suspendisse ut scelerisque tellus. In ac quam sem.Curabitur suscipit massa nisl. Ut et metus sed lacus dapibus molestie. Nullam\n"
		"porttitor sit amet magna quis dapibus. Nulla tincidunt, arcu sit amet hendrerit consequat, felis leo blandit libero, eu posuere\n"
		"nisl quam interdum nulla. Quisque nec efficitur libero. Quisque quis orci vitae metus feugiat aliquam eu et nulla. Etiam aliquet\n"
		"ante vitae lacus aliquam, et gravida elit mollis. Proin molestie, justo tempus rhoncus aliquam, tellus erat venenatis erat,\n"
		"porttitor dapibus nunc purus id enim. Integer a nunc ut velit porta maximus. Nullam rutrum nisi in sagittis pharetra. Proin id\n"
		"pharetra augue, sed vulputate lorem. Aenean dapibus, turpis nec ullamcorper pharetra, ex augue congue nibh, condimentum\n"
		"vestibulum arcu urna quis ex.\n\n"
		"Vestibulum non dignissim nibh, quis vestibulum odio. Ut sed viverra ante, fringilla convallis tellus. Donec in est rutrum,\n"
		"imperdiet dolor a, vestibulum magna. In nec justo tellus. Ut non erat eu leo ornare imperdiet in sit amet lorem. Nullam quis\n"
		"nisl diam. Aliquam laoreet dui et ligula posuere cursus.\n\n"
		"Donec vestibulum ante eget arcu dapibus lobortis.Curabitur condimentum tellus felis, id luctus mi ultrices quis. Aenean nulla\n"
		"justo, venenatis vel risus et, suscipit faucibus nulla. Pellentesque habitant morbi tristique senectus et netus et malesuada\n"
		"fames ac turpis egestas. Sed lacinia metus eleifend lacinia blandit.Morbi est nibh, dapibus nec arcu quis, volutpat lacinia\n"
		"dolor. Vestibulum quis viverra erat.Maecenas ultricies odio neque, et eleifend arcu auctor in. Suspendisse placerat massa nisl,\n"
		"non condimentum ligula sodales at.Phasellus eros urna, elementum in ultricies quis, vulputate id magna. Donec efficitur rutrum\n"
		"urna sed tempus. Vestibulum eu augue dolor. Vestibulum vehicula suscipit purus, sit amet ultricies ligula malesuada sit amet.\n"
		"Duis consectetur elit euismod arcu aliquet vehicula. Pellentesque lobortis dui et nisl vehicula, in placerat quam dapibus.Fusce\n"
		"auctor arcu a purus bibendum, eget blandit nisi lobortis.");
}

void Pannable::UpdateObject()
{
	GetTransform().SetSize(Window::Get()->GetSize());

	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));

	Vector2f offset = GetTransform().GetPosition();

	m_zoom *= powf(1.3f, 0.1f * Mouse::Get()->GetWheelDelta().m_y);
	dynamic_cast<DriverConstant<Vector2f> *>(GetScaleDriver())->SetConstant(Vector2f(m_zoom));

	if (Mouse::Get()->GetButton(MouseButton::Left) != InputAction::Release)
	{
		offset -= Mouse::Get()->GetDelta() / m_zoom / Engine::Get()->GetDelta().AsSeconds();
	}

	GetTransform().SetPosition(offset);
}
}
