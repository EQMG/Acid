#include "Pannable.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Renderer/Renderer.hpp>
#include <Uis/Uis.hpp>

namespace test
{
	Pannable::Pannable(UiObject *parent) :
		UiObject(parent, UiBound::Screen),
		m_buttonReset(ButtonKeyboard({Key::Enter})),
		m_zoom(1.0f),
		m_timerUpdate(Timer(Time::Seconds(0.333f))),
		m_background(std::make_unique<Gui>(parent, UiBound::Screen, Texture::Create("Guis/White.png"))),
		m_texts(std::vector<std::unique_ptr<Text>>()),
		m_textFps(std::make_unique<Text>(parent, UiBound(Vector2(0.002f, 0.978f), UiReference::BottomLeft), 1.1f, "FPS: 0", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left)),
		m_textUps(std::make_unique<Text>(parent, UiBound(Vector2(0.002f, 0.998f), UiReference::BottomLeft), 1.1f, "UPS: 0", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left))
	{
		m_texts.emplace_back(std::make_unique<Text>(this, UiBound(Vector2(0.5f, -0.7f), UiReference::Centre), 6.0f, "Acid Font",
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, 1.0f, Colour::Red, 0.0f, 0.015f));

		static const std::string content1 =
			"@&(3 Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus sit amet scelerisque augue, sit amet commodo neque. Vestibulum \n"
			"eu eros a justo molestie bibendum quis in urna. Integer quis tristique magna. Morbi in ultricies lorem. Donec lacinia nisi et \n"
			"arcu scelerisque, eget viverra ante dapibus. Proin enim neque, vehicula id congue quis, consequat sit amet tortor.Aenean ac \n"
			"lorem sit amet magna rhoncus rhoncus ac ac neque. Cras sed rutrum sem. Donec placerat ultricies ex, a gravida lorem commodo ut. \n"
			"Mauris faucibus aliquet ligula, vitae condimentum dui semper et. Aenean pellentesque ac ligula a varius. Suspendisse congue \n"
			"lorem lorem, ac consectetur ipsum condimentum id. \n\n"
			"Vestibulum quis erat sem. Fusce efficitur libero et leo sagittis, ac volutpat felis ullamcorper. Curabitur fringilla eros eget ex \n"
			"lobortis, at posuere sem consectetur. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis \n"
			"egestas. Vivamus eu enim leo. Morbi ultricies lorem et pellentesque vestibulum. Proin eu ultricies sem. Quisque laoreet, ligula \n"
			"non molestie congue, odio nunc tempus arcu, vel aliquet leo turpis non enim. Sed risus dui, condimentum et eros a, molestie \n"
			"imperdiet nisl. Vivamus quis ante venenatis, cursus magna ut, tincidunt elit. Aenean nisl risus, porttitor et viverra quis, \n"
			"tempus vitae nisl. \n\n"
			"Suspendisse ut scelerisque tellus. In ac quam sem. Curabitur suscipit massa nisl. Ut et metus sed lacus dapibus molestie. nullptram \n"
			"porttitor sit amet magna quis dapibus. nullptra tincidunt, arcu sit amet hendrerit consequat, felis leo blandit libero, eu posuere \n"
			"nisl quam interdum nullptra. Quisque nec efficitur libero. Quisque quis orci vitae metus feugiat aliquam eu et nullptra. Etiam aliquet \n"
			"ante vitae lacus aliquam, et gravida elit mollis. Proin molestie, justo tempus rhoncus aliquam, tellus erat venenatis erat, \n"
			"porttitor dapibus nunc purus id enim. Integer a nunc ut velit porta maximus. nullptram rutrum nisi in sagittis pharetra. Proin id \n"
			"pharetra augue, sed vulputate lorem. Aenean dapibus, turpis nec ullamcorper pharetra, ex augue congue nibh, condimentum \n"
			"vestibulum arcu urna quis ex. \n\n"
			"Vestibulum non dignissim nibh, quis vestibulum odio. Ut sed viverra ante, fringilla convallis tellus. Donec in est rutrum, \n"
			"imperdiet dolor a, vestibulum magna. In nec justo tellus. Ut non erat eu leo ornare imperdiet in sit amet lorem. nullptram quis \n"
			"nisl diam. Aliquam laoreet dui et ligula posuere cursus. \n\n"
			"Donec vestibulum ante eget arcu dapibus lobortis.Curabitur condimentum tellus felis, id luctus mi ultrices quis. Aenean nullptra \n"
			"justo, venenatis vel risus et, suscipit faucibus nullptra. Pellentesque habitant morbi tristique senectus et netus et malesuada \n"
			"fames ac turpis egestas. Sed lacinia metus eleifend lacinia blandit. Morbi est nibh, dapibus nec arcu quis, volutpat lacinia \n"
			"dolor. Vestibulum quis viverra erat. Maecenas ultricies odio neque, et eleifend arcu auctor in. Suspendisse placerat massa nisl, \n"
			"non condimentum ligula sodales at. Phasellus eros urna, elementum in ultricies quis, vulputate id magna. Donec efficitur rutrum \n"
			"urna sed tempus. Vestibulum eu augue dolor. Vestibulum vehicula suscipit purus, sit amet ultricies ligula malesuada sit amet. \n"
			"Duis consectetur elit euismod arcu aliquet vehicula. Pellentesque lobortis dui et nisl vehicula, in placerat quam dapibus. Fusce \n"
			"auctor arcu a purus bibendum, eget blandit nisi lobortis.";
		m_texts.emplace_back(std::make_unique<Text>(this, UiBound(Vector2(0.5f, 0.0f), UiReference::Centre), 1.8f, content1,
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, 10.0f, Colour::Black, 0.0f, 0.015f));
	}

	void Pannable::UpdateObject()
	{
		if (m_timerUpdate.IsPassedTime())
		{
			m_timerUpdate.ResetStartTime();

			m_textFps->SetString("FPS: " + String::To(static_cast<int>(1.0f / Engine::Get()->GetDeltaRender().AsSeconds())));
			m_textUps->SetString("UPS: " + String::To(static_cast<int>(1.0f / Engine::Get()->GetDelta().AsSeconds())));
		}

		// TODO: Cleanup offset and implement zoom.
		Vector2 offset = GetRectangle().GetPosition();

		if (m_buttonReset.WasDown())
		{
			m_zoom = 1.0f; 
			offset = Vector2(0.0f, 0.0f);
		}

		m_zoom *= powf(1.3f, 0.15f * Mouse::Get()->GetDeltaWheel());

		if (Mouse::Get()->GetButton(MouseButton::Left) != InputAction::Release)
		{
			offset.m_x -= Mouse::Get()->GetDeltaX() * Window::Get()->GetAspectRatio() / m_zoom / Engine::Get()->GetDelta().AsSeconds();
			offset.m_y -= Mouse::Get()->GetDeltaY() / m_zoom / Engine::Get()->GetDelta().AsSeconds();
		}

		GetRectangle().SetPosition(offset);
	}
}
