#pragma once

#include <flounder.hpp>

using namespace Flounder;

class overlaydebug :
	public uiobject
{
private:
	text *m_textHeight;
	text *m_textTime;
	text *m_textPosition;
	text *m_textFps;
	text *m_textUps;
	timer *m_timerUpdate;
public:
	overlaydebug(uiobject *parent);

	~overlaydebug();

	virtual void updateObject() override;
private:
	text *createStatus(const std::string &content, const float &positionX, const float &positionY, const uialign &align);
};
