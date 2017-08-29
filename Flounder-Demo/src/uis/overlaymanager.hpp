#pragma once

#include <flounder.hpp>

using namespace flounder;

class overlaymanager :
	public uiobject
{
private:
	inputbutton *m_inputbutton;
	inputslider *m_inputslider;
	inputtext *m_inputtext;
	inputgrabber *m_inputjoystick;
	inputgrabber *m_inputkeyboard;
	inputgrabber *m_inputmouse;
public:
	overlaymanager(uiobject *parent);

	~overlaymanager();

	virtual void updateObject() override;
};
