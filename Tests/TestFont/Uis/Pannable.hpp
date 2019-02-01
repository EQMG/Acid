#pragma once

#include <Inputs/ButtonKeyboard.hpp>
#include <Uis/UiObject.hpp>
#include <Maths/Timer.hpp>
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

		virtual void UpdateObject() override;
	private:
		ButtonKeyboard m_buttonReset;

		float m_zoom;
		Timer m_timerUpdate;

		std::unique_ptr<Gui> m_background;
		std::vector<std::unique_ptr<Text>> m_texts;

		std::unique_ptr<Text> m_textFps;
		std::unique_ptr<Text> m_textUps;
	};
}
