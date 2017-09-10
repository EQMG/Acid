#pragma once

#include <Flounder.hpp>

using namespace Flounder;

class OverlayManager :
	public UiObject
{
private:
	InputButton *m_inputbutton;
	InputSlider *m_inputslider;
	InputText *m_inputtext;
	InputGrabber *m_inputjoystick;
	InputGrabber *m_inputkeyboard;
	InputGrabber *m_inputmouse;
public:
	OverlayManager(UiObject *parent);

	~OverlayManager();

	virtual void UpdateObject() override;
};
