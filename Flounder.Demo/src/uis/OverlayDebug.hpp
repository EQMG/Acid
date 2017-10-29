#pragma once

#include <Flounder.hpp>

using namespace Flounder;

namespace Demo
{
	class OverlayDebug :
		public UiObject
	{
	private:
	//	Text *m_textHeight;
	//	Text *m_textTime;
	//	Text *m_textPosition;
		Text *m_textFps;
		Text *m_textUps;
		Timer *m_timerUpdate;
	public:
		OverlayDebug(UiObject *parent);

		~OverlayDebug();

		virtual void UpdateObject() override;
	private:
		Text *CreateStatus(const std::string &content, const float &positionX, const float &positionY, const Justify &justify);
	};
}
