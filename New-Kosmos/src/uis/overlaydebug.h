#pragma once

#include <flounder.h>
#include "uis/inputbutton.h"
#include "uis/inputtext.h"
#include "uis/inputgrabber.h"
#include "uis/inputslider.h"

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
	inputgrabber *m_inputmouse;
	inputgrabber *m_inputkeyboard;
public:
	overlaydebug(uiobject *parent);

	~overlaydebug();

	virtual void updateObject() override;
private:
	text *createStatus(const std::string &content, const float &positionY);
};
