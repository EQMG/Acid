#pragma once

#include <flounder.h>

using namespace flounder;

class overlaydebug :
	public uiobject
{
private:
	text *m_textFps;
	text *m_textUps;
	timer *m_timerUpdate;

	inputbutton *m_inputbutton;
	inputslider *m_inputslider;
	inputtext *m_inputtext;
	inputgrabber *m_inputjoystick;
	inputgrabber *m_inputkeyboard;
	inputgrabber *m_inputmouse;
public:
	overlaydebug(uiobject *parent);

	~overlaydebug();

	virtual void updateObject() override;
private:
	text *createStatus(const std::string &content, const float &positionY);
};
