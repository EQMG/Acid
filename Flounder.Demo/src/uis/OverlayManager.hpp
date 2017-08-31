#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class OverlayManager :
	public UiObject
{
private:
	inputbutton *m_inputbutton;
	inputslider *m_inputslider;
	inputtext *m_inputtext;
	inputgrabber *m_inputjoystick;
	inputgrabber *m_inputkeyboard;
	inputgrabber *m_inputmouse;
public:
	OverlayManager(UiObject *parent);

	~OverlayManager();

	virtual void UpdateObject() override;
};
