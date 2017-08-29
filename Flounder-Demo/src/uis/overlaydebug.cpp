#include "overlaydebug.hpp"

overlaydebug::overlaydebug(uiobject *parent) :
	uiobject(parent, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f))
{
	m_textHeight = createStatus("HEIGHT: 0.0", 0.005f, 0.86f, LEFT);
	m_textTime = createStatus("TIME: 0.0", 0.005f, 0.89f, LEFT);
	m_textPosition = createStatus("POSITION: 0.0, 0.0, 0.0", 0.005f, 0.92f, LEFT);
	m_textFps = createStatus("FPS: 0", 0.005f, 0.95f, LEFT);
	m_textUps = createStatus("UPS: 0", 0.005f, 0.98f, LEFT);
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

		if (terrains::get() != nullptr)
		{
			float terrainHeight = 0.0f; //  terrains::get()->getTerrain()->getHeight(position->m_x, position->m_z);
			m_textHeight->setText("HEIGHT: " + std::to_string(maths::roundToPlace(terrainHeight, 2)));
		}

		if (worlds::get() != nullptr)
		{
			m_textTime->setText("TIME: " + std::to_string(static_cast<int>(worlds::get()->getDayFactor() * 100.0f)) + "%");
		}

		if (camera::get() != nullptr)
		{
			vector3 *position = camera::get()->getCamera()->getPosition();
			m_textPosition->setText("POSITION: " + std::to_string(static_cast<int>(position->m_x)) + ", " + std::to_string(static_cast<int>(position->m_y)) + ", " + std::to_string(static_cast<int>(position->m_z)));
		}

		m_textFps->setText("FPS: " + std::to_string(static_cast<int>(1.0 / framework::get()->getDeltaRender())));
		m_textUps->setText("UPS: " + std::to_string(static_cast<int>(1.0 / framework::get()->getDelta())));
	}
}

text *overlaydebug::createStatus(const std::string &content, const float &positionX, const float &positionY, const uialign &align)
{
	text *result = new text(this, vector2(positionX, positionY), content, 1.0f, uis::get()->candara, 1.0f, align);
	result->setInScreenCoords(true);
	result->setTextColour(colour(1.0f, 1.0f, 1.0f));
	result->setBorderColour(colour(0.15f, 0.15f, 0.15f));
	result->setBorder(new driverconstant(0.04f));
	return result;
}
