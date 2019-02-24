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

		Gui m_background;
		Text m_title;
		Text m_body;

		Text m_textFrameTime;
		Text m_textFps;
		Text m_textUps;
	};
}
