#include "overlaymanager.hpp"

OverlayManager::OverlayManager(UiObject *parent) :
	UiObject(parent, Vector2(0.5f, 0.5f), Vector2(1.0f, 1.0f))
{
	m_inputbutton = new inputbutton(this, Vector2(0.5f, 0.3f), "Button", CENTRE);
	m_inputbutton->setActionLeft([&]()
	{
		printf("Sample button pressed!");
	});
	m_inputbutton->SetAlphaDriver(new driverconstant(1.0f));

	m_inputslider = new inputslider(this, Vector2(0.5f, 0.37f), "Slider", 0.0f, 1.0f, 0.333f, CENTRE);
	m_inputslider->setActionChange([&]()
	{
	});
	m_inputslider->SetAlphaDriver(new driverconstant(1.0f));

	m_inputtext = new inputtext(this, Vector2(0.5f, 0.44f), "Input: ", "value", CENTRE);
	m_inputtext->setActionChange([&]()
	{
	});
	m_inputtext->SetAlphaDriver(new driverconstant(1.0f));

	m_inputjoystick = new inputgrabber(this, Vector2(0.5f, 0.51f), "Joystick Key: ", 3, new grabberjoystick(0), CENTRE);
	m_inputjoystick->setActionChange([&]()
	{
	});
	m_inputjoystick->SetAlphaDriver(new driverconstant(1.0f));

	m_inputkeyboard = new inputgrabber(this, Vector2(0.5f, 0.58f), "Keyboard Key: ", 'w', new grabberkeyboard(), CENTRE);
	m_inputkeyboard->setActionChange([&]()
	{
	});
	m_inputkeyboard->SetAlphaDriver(new driverconstant(1.0f));

	m_inputmouse = new inputgrabber(this, Vector2(0.5f, 0.65f), "Mouse Key: ", 0, new grabbermouse(), CENTRE);
	m_inputmouse->setActionChange([&]()
	{
	});
	m_inputmouse->SetAlphaDriver(new driverconstant(0.0f));
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
