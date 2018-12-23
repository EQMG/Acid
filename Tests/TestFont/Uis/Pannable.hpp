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
	private:
		ButtonKeyboard m_buttonReset;

		float m_zoom;
		Vector2 m_offset;
		Timer m_timerUpdate;

		std::unique_ptr<Gui> m_background;
		std::vector<std::pair<Vector3, std::unique_ptr<Text>>> m_texts;

		std::unique_ptr<Text> m_textFps;
		std::unique_ptr<Text> m_textUps;
	public:
		explicit Pannable(UiObject *parent);

		virtual void UpdateObject() override;
	};
}
