#include "Pannable.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Renderer/Renderer.hpp>
#include <Uis/Uis.hpp>

namespace test
{
Pannable::Pannable(UiObject *parent) :
	UiObject(parent, UiBound::Maximum),
	m_buttonReset(ButtonKeyboard({Key::Enter})),
//	m_testCompound(ButtonCompound::Create<ButtonKeyboard>(true, Key::G, Key::H, Key::J)),
//	m_testHat(0, 0, JoystickHat::Up | JoystickHat::Right),
	m_settings(parent, UiBound(Vector2(0.02f, 0.02f), UiReference::TopLeft, UiAspect::Dimensions, Vector2(0.25f, 0.2f)), ScrollBar::None),
	m_masterVolume(&m_settings.GetContent(), "Master Volume", 100.0f, 0.0f, 100.0f, 0, UiBound(Vector2(0.5f, 0.06f), UiReference::TopCentre)),
	m_antialiasing(&m_settings.GetContent(), "Antialiasing", true, UiBound(Vector2(0.5f, 0.30f), UiReference::TopCentre)),
	m_zoom(1.0f),
	m_title(this, UiBound(Vector2(0.5f, -0.7f), UiReference::Centre, UiAspect::Position | UiAspect::Dimensions | UiAspect::Scale), 6.0f, "Acid Font",
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, 1.0f, Colour::Red, 0.0f, 0.015f),
	m_body(this, UiBound(Vector2(0.5f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Dimensions | UiAspect::Scale), 1.8f, "",
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, 10.0f, Colour::Black, 0.002f, 0.015),
	m_textFrameTime(parent, UiBound(Vector2(0.002f, 0.998f), UiReference::BottomLeft), 1.1f, "Frame Time: 0ms", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left),
	m_textFps(parent, UiBound(Vector2(0.002f, 0.978f), UiReference::BottomLeft), 1.1f, "FPS: 0", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left),
	m_textUps(parent, UiBound(Vector2(0.002f, 0.958f), UiReference::BottomLeft), 1.1f, "UPS: 0", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left)
{
	m_buttonReset.GetOnButton() += [this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			m_zoom = 1.0f;
			GetRectangle().SetPosition(Vector2(0.5f, 0.5f));
		}
		Log::Out("Button Reset: %i\n", action);
	};
	/*m_testCompound->GetOnButton() += [this](InputAction action, BitMask<InputMod> mods)
	{
		Log::Out("Test Compound: %i\n", action);
	};
	m_testHat.GetOnButton() += [this](InputAction action, BitMask<InputMod> mods)
	{
		Log::Out("Test Hat: %i\n", action);
	};*/

	m_settings.SetHeight(4.0f);
	m_masterVolume.GetOnValue() += [this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Master, value / 100.0f);
	};
	m_antialiasing.GetOnValue() += [this](bool value)
	{
	};

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
	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));

	Vector2 offset = GetRectangle().GetPosition();

	m_zoom *= powf(1.3f, 0.1f * Mouse::Get()->GetDeltaWheel());
	dynamic_cast<DriverConstant<float> *>(GetScaleDriver())->SetConstant(m_zoom);

	if (Mouse::Get()->GetButton(MouseButton::Left) != InputAction::Release)
	{
		offset -= Mouse::Get()->GetDelta() / m_zoom / Engine::Get()->GetDelta().AsSeconds();
	}

	GetRectangle().SetPosition(offset);
}
}
