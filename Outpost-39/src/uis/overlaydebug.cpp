#include "overlaydebug.h"

overlaydebug::overlaydebug(uiobject *parent) :
	uiobject(parent, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f))
{
	m_textText = createStatus("People: 100      Oxygen:150      Water: 172      Food: 56            WARNING", 0.5f, 0.03f, CENTRE);
	m_textFps = createStatus("FPS: 0", 0.01f, 0.94f, LEFT);
	m_textUps = createStatus("UPS: 0", 0.01f, 0.97f, LEFT);
	m_timerUpdate = new timer(0.333f);

	m_inputbutton = new inputbutton(this, vector2(0.5f, 0.3f), "Button", CENTRE);
	m_inputbutton->setActionLeft([&]()
	{
		std::cout << "Sample button pressed!" << std::endl;
	});
	m_inputbutton->setAlphaDriver(new driverconstant(0.0f));

	m_inputslider = new inputslider(this, vector2(0.5f, 0.37f), "Slider", 0.0f, 1.0f, 0.333f, CENTRE);
	m_inputslider->setActionChange([&]()
	{
	});
	m_inputslider->setAlphaDriver(new driverconstant(0.0f));

	m_inputtext = new inputtext(this, vector2(0.5f, 0.44f), "Input: ", "value", CENTRE);
	m_inputtext->setActionChange([&]()
	{
	});
	m_inputtext->setAlphaDriver(new driverconstant(0.0f));

	m_inputjoystick = new inputgrabber(this, vector2(0.5f, 0.51f), "Joystick Key: ", 3, new grabberjoystick(0), CENTRE);
	m_inputjoystick->setActionChange([&]()
	{
	});
	m_inputjoystick->setAlphaDriver(new driverconstant(0.0f));

	m_inputkeyboard = new inputgrabber(this, vector2(0.5f, 0.58f), "Keyboard Key: ", 'w', new grabberkeyboard(), CENTRE);
	m_inputkeyboard->setActionChange([&]()
	{
	});
	m_inputkeyboard->setAlphaDriver(new driverconstant(0.0f));

	m_inputmouse = new inputgrabber(this, vector2(0.5f, 0.65f), "Mouse Key: ", 0, new grabbermouse(), CENTRE);
	m_inputmouse->setActionChange([&]()
	{
	});
	m_inputmouse->setAlphaDriver(new driverconstant(0.0f));
}

overlaydebug::~overlaydebug()
{
	delete m_textFps;
	delete m_textUps;

	delete m_inputbutton;
	delete m_inputslider;
	delete m_inputtext;
	delete m_inputjoystick;
	delete m_inputkeyboard;
	delete m_inputmouse;
}

void overlaydebug::updateObject()
{
	if (m_timerUpdate->isPassedTime())
	{
		m_timerUpdate->resetStartTime();

		m_textFps->setText("FPS: " + std::to_string(static_cast<int>(1.0 / framework::get()->getDeltaRender())));
		m_textUps->setText("UPS: " + std::to_string(static_cast<int>(1.0 / framework::get()->getDelta())));
	}
}

text *overlaydebug::createStatus(const std::string &content, const float &positionX, const float &positionY, const uialign &align)
{
	text *result = new text(this, vector2(positionX, 0.01f + positionY), content, 1.0f, uis::get()->candara, 1.0f, align);
	result->setInScreenCoords(true);
	result->setTextColour(colour(1.0f, 1.0f, 1.0f));
	result->setBorderColour(colour(0.15f, 0.15f, 0.15f));
	result->setBorder(new driverconstant(0.04f));
	return result;
}
