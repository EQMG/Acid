#include "OverlayManager.hpp"

namespace Demo
{
	OverlayManager::OverlayManager(UiObject *parent) :
		UiObject(parent, Vector2(0.5f, 0.5f), Vector2(1.0f, 1.0f), Vector2(0.5f, 0.5f), true)
	{
		m_inputbutton = new InputButton(this, Vector2(0.5f, 0.3f), "Button", JustifyCentre);
		m_inputbutton->SetActionLeft([&]()
		{
			printf("Sample button pressed!");
		});
		m_inputbutton->SetAlphaDriver(new DriverConstant(1.0f));

		m_inputslider = new InputSlider(this, Vector2(0.5f, 0.37f), "Slider", 0.0f, 1.0f, 0.333f, JustifyCentre);
		m_inputslider->SetActionChange([&]()
		{
		});
		m_inputslider->SetAlphaDriver(new DriverConstant(1.0f));

		m_inputtext = new InputText(this, Vector2(0.5f, 0.44f), "Input: ", "value", JustifyCentre);
		m_inputtext->SetActionChange([&]()
		{
		});
		m_inputtext->SetAlphaDriver(new DriverConstant(1.0f));

		m_inputjoystick = new InputGrabber(this, Vector2(0.5f, 0.51f), "Joystick Key: ", 3, new GrabberJoystick(0), JustifyCentre);
		m_inputjoystick->SetActionChange([&]()
		{
		});
		m_inputjoystick->SetAlphaDriver(new DriverConstant(1.0f));

		m_inputkeyboard = new InputGrabber(this, Vector2(0.5f, 0.58f), "Keyboard Key: ", 'w', new GrabberKeyboard(), JustifyCentre);
		m_inputkeyboard->SetActionChange([&]()
		{
		});
		m_inputkeyboard->SetAlphaDriver(new DriverConstant(1.0f));

		m_inputmouse = new InputGrabber(this, Vector2(0.5f, 0.65f), "Mouse Key: ", 0, new GrabberMouse(), JustifyCentre);
		m_inputmouse->SetActionChange([&]()
		{
		});
		m_inputmouse->SetAlphaDriver(new DriverConstant(0.0f));
	}

	OverlayManager::~OverlayManager()
	{
		delete m_inputbutton;
		delete m_inputslider;
		delete m_inputtext;
		delete m_inputjoystick;
		delete m_inputkeyboard;
		delete m_inputmouse;
	}

	void OverlayManager::UpdateObject()
	{
	}
}
