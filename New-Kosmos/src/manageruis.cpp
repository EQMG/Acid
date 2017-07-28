#include "manageruis.h"

const float manageruis::SLIDE_TIME = 0.5f;

manageruis::manageruis() :
	imanageruis()
{
	// m_primaryColour = new colour(0.90196078431f, 0.08235294117f, 0.08235294117f); // Charger Red.
	// m_primaryColour = new colour(0.1f, 0.8f, 0.2f); // Neon Green.
	// m_primaryColour = new colour(0.0824f, 0.396f, 0.753f); // Water Blue.
	m_primaryColour = new colour(0.2f, 0.2f, 1.0f); // Some Blue.

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
	return true; // m_overlayStartup->isStarting()
}

float manageruis::getBlurFactor()
{
	return 0.0f;
}
