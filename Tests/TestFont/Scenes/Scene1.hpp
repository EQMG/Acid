#pragma once

#include <Scenes/Scene.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test
{
	class Scene1 :
		public Scene
	{
	private:
		ButtonKeyboard m_buttonFullscreen;
		ButtonKeyboard m_buttonScreenshot;
		ButtonKeyboard m_buttonReset;
		ButtonKeyboard m_buttonExit;

		float m_zoom;
		Vector2 m_offset;
		Timer m_timerUpdate;

		std::unique_ptr<Gui> m_background;
		std::vector<std::pair<Vector2, std::unique_ptr<Text>>> m_texts;

		std::unique_ptr<Text> m_textFps;
		std::unique_ptr<Text> m_textUps;
	public:
		Scene1();

		void Start() override;

		void Update() override;

		bool IsPaused() const override;
	};
}
