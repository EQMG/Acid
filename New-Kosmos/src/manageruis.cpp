#include "manageruis.h"

const float manageruis::SLIDE_TIME = 0.5f;

manageruis::manageruis() :
	imanageruis()
{
	m_primaryColour = new colour(1.0f, 0.0f, 0.0f);
	m_overlayStartup = new overlaystartup(uis::get()->getContainer());
	m_overlayDebug = new overlaydebug(uis::get()->getContainer());

	m_overlayStartup->setAlphaDriver(new driverconstant(1.0f));
	m_overlayDebug->setAlphaDriver(new driverconstant(0.0f));

	uis::get()->getSelector()->load(0, 0, 1, 0, 1);
}

manageruis::~manageruis()
{
	delete m_primaryColour;
	delete m_overlayStartup;
	delete m_overlayDebug;
}

void manageruis::update()
{
	if (m_overlayStartup->getAlpha() == 0.0f && m_overlayStartup->isStarting())
	{
		m_overlayStartup->setAlphaDriver(new driverconstant(0.0f));
		m_overlayDebug->setAlphaDriver(new driverslide(0.0f, 1.0f, SLIDE_TIME));
		m_overlayStartup->setStarting(false);
	}
}

bool manageruis::isGamePaused()
{
	return m_overlayStartup->isStarting();
}

float manageruis::getBlurFactor()
{
	return 0.0f;
}
