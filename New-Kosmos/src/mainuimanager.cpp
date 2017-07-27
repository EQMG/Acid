#include "mainuimanager.h"

const float mainuimanager::SLIDE_TIME = 0.5f;

mainuimanager::mainuimanager() :
	iuimanager()
{
	m_primaryColour = new colour(1.0f, 0.0f, 0.0f);
	m_overlayStartup = new overlaystartup(uis::get()->getContainer());
	m_overlayDebug = new overlaydebug(uis::get()->getContainer());

	m_overlayStartup->setAlphaDriver(new driverconstant(1.0f));
	m_overlayDebug->setAlphaDriver(new driverconstant(0.0f));

	uis::get()->getSelector()->load(0, 0, 1, 0, 1);
}

mainuimanager::~mainuimanager()
{
	delete m_primaryColour;
	delete m_overlayStartup;
	delete m_overlayDebug;
}

void mainuimanager::update()
{
	if (m_overlayStartup->getAlpha() == 0.0f && m_overlayStartup->isStarting())
	{
		m_overlayStartup->setAlphaDriver(new driverconstant(0.0f));
		m_overlayDebug->setAlphaDriver(new driverslide(0.0f, 1.0f, SLIDE_TIME));
		m_overlayStartup->setStarting(false);
	}
}

bool mainuimanager::isGamePaused()
{
	return m_overlayStartup->isStarting();
}

float mainuimanager::getBlurFactor()
{
	return 0.0f;
}
