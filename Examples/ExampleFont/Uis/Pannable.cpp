#include "Pannable.hpp"

#include <Inputs/Inputs.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>
#include <Uis/Inputs/UiButtonInput.hpp>
#include <Graphics/Graphics.hpp>
#include <Uis/Uis.hpp>

import acid.sound;

namespace test {
Pannable::Pannable(){
	//content.SetTransform({{1000, 1000}, UiAnchor::LeftTop, {0.5f, 0.5f}});
	AddChild(&content);

	//title.SetTransform({{300, 80}, UiAnchor::CentreTop});
	title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	title.SetFontSize(72);
	title.SetJustify(Text::Justify::Centre);
	title.SetTextColour(Colour::Red);
	title.SetString("Acid Font");
	content.AddChild(&title);

	//body.SetTransform({{750, 1000}, UiAnchor::CentreTop, {0, 100}});
	body.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	body.SetTextColour(Colour::Black);
	body.SetString( //L"Hello world, Привет мир, schön! 0123456789 #$%^*@&( []{} «»½¼±¶§\n"
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
	content.AddChild(&body);

	//settings.SetTransform({{300, 300}, UiAnchor::LeftTop, {20, 20}});
	//settings.GetTransform().SetDepth(-4.0f);
	settings.SetManipulate(UiManipulate::All);
	settings.SetScrollBars(ScrollBar::None);
	settings.SetBackgroundColor(UiButtonInput::BackgroundColour);
	AddChild(&settings);

	//masterVolume.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, 0}});
	masterVolume.SetTitle("Master Volume");
	masterVolume.SetValueMin(0.0f);
	masterVolume.SetValueMax(100.0f);
	masterVolume.SetRoundTo(0);
	masterVolume.SetValue(100.0f);
	masterVolume.OnValue().connect(this, [this](float value) {
		Audio::Get()->SetGain(Audio::Type::Master, value / 100.0f);
	});
	settings.AddChild(&masterVolume);

	//antialiasing.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, 34}});
	antialiasing.SetTitle("Antialiasing");
	antialiasing.SetValue(true);
	antialiasing.OnValue().connect(this, [this](bool value) {
	});
	settings.AddChild(&antialiasing);
	
	//textFrameTime.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -2}});
	textFrameTime.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textFrameTime.SetFontSize(11);
	AddChild(&textFrameTime);

	//textFps.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -16}});
	textFps.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textFps.SetFontSize(11);
	AddChild(&textFps);

	//textUps.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -30}});
	textUps.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textUps.SetFontSize(11);
	AddChild(&textUps);

	Inputs::Get()->GetButton("reset")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			zoom = 1.0f;
//			content.GetTransform().SetPosition({0.5f, 0.5f});
		}

		Log::Out("Button Reset: ", static_cast<uint32_t>(action), '\n'); // TODO: Enum stream operators.
	});
}

void Pannable::UpdateObject() {
	textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));

//	auto offset = content.GetTransform().GetPosition();

	zoom += Windows::Get()->GetWindow(0)->GetMouseScrollDelta().y;
	dynamic_cast<ConstantDriver<Vector2f> *>(content.GetScaleDriver())->SetConstant(Vector2f(zoom));

	if (Windows::Get()->GetWindow(0)->GetMouseButton(MouseButton::Left) != InputAction::Release) {
//		offset -= Mouse::Get()->GetPositionDelta() / zoom / Engine::Get()->GetDelta().AsSeconds();
	}

//	content.GetTransform().SetPosition(offset);
}
}
