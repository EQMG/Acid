#pragma once

#include <Inputs/ButtonKeyboard.hpp>
#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test
{
	class Pannable :
		public UiObject
	{
	public:
		explicit Pannable(UiObject *parent);

		void UpdateObject() override;
	private:
		ButtonKeyboard m_buttonReset;

		float m_zoom;

		std::unique_ptr<Gui> m_background;
		std::vector<std::unique_ptr<Text>> m_texts;

		std::unique_ptr<Text> m_textFrameTime;
		std::unique_ptr<Text> m_textFps;
		std::unique_ptr<Text> m_textUps;
	};
}
