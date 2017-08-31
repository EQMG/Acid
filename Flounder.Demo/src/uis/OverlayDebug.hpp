#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class OverlayDebug :
	public UiObject
{
private:
	text *m_textHeight;
	text *m_textTime;
	text *m_textPosition;
	text *m_textFps;
	text *m_textUps;
	Timer *m_timerUpdate;
public:
	OverlayDebug(UiObject *parent);

	~OverlayDebug();

	virtual void UpdateObject() override;
private:
	text *CreateStatus(const std::string &content, const float &positionX, const float &positionY, const uialign &align);
};
