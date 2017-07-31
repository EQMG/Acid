#include "overlaydebug.h"

overlaydebug::overlaydebug(uiobject *parent) :
	uiobject(parent, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f))
{
	m_textFps = createStatus("FPS: 0", 0.01f, 0.94f, LEFT);
	m_textUps = createStatus("UPS: 0", 0.01f, 0.97f, LEFT);
	m_timerUpdate = new timer(0.333f);
}

overlaydebug::~overlaydebug()
{
	delete m_textFps;
	delete m_textUps;
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
