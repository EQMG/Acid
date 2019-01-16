#pragma once

#include <Uis/UiObject.hpp>
#include <Maths/Timer.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test
{
	class OverlayDebug :
		public UiObject
	{
	private:
		std::unique_ptr<Text> m_textFps;
		std::unique_ptr<Text> m_textUps;
		Timer m_timerUpdate;
	public:
		explicit OverlayDebug(UiObject *parent);

		virtual void UpdateObject() override;
	private:
		std::unique_ptr<Text> CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Text::Justify &justify);
	};
}
