#include "mainguimanager.h"

mainguimanager::mainguimanager() :
	iguimanager()
{
	m_primaryColour = new colour(1.0f, 0.0f, 0.0f);
	m_overlayStartup = new overlaystartup(guis::get()->getContainer());

	m_overlayStartup->setAlphaDriver(new driverconstant(1.0f));

	guis::get()->getSelector()->load(0, 0, 1, 0, 1);
}

mainguimanager::~mainguimanager()
{
	delete m_primaryColour;
}

void mainguimanager::update()
{
	if (m_overlayStartup->getAlpha() == 0.0f && m_overlayStartup->getStarting())
	{
		m_overlayStartup->setAlphaDriver(new driverconstant(0.0f));
		m_overlayStartup->setStarting(false);
	}
}

bool mainguimanager::isGamePaused()
{
	return m_overlayStartup->getStarting();
}

float mainguimanager::getBlurFactor()
{
	return 0.0f;
}
