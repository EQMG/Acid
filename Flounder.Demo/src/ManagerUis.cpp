#include "manageruis.hpp"

const float ManagerUis::SLIDE_TIME = 0.5f;

ManagerUis::ManagerUis() :
	IManagerUis(),
	m_primaryColour(new Colour(0.91f, 0.08f, 0.08f, 1.0f)),
	m_overlayStartup(new overlaystartup(uis::get()->getContainer())),
	m_overlayDebug(new OverlayDebug(uis::get()->getContainer())),
	m_overlayManager(new OverlayManager(uis::get()->getContainer()))
{
	uis::get()->getSelector()->load(0, 0, 1, 0, 1);

	m_overlayStartup->SetAlphaDriver(new driverconstant(1.0f));
	m_overlayDebug->SetAlphaDriver(new driverconstant(0.0f));
	m_overlayManager->SetAlphaDriver(new driverconstant(0.0f));
}

ManagerUis::~ManagerUis()
{
	delete m_primaryColour;
	delete m_overlayStartup;
	delete m_overlayDebug;
	delete m_overlayManager;
}

void ManagerUis::Update()
{
	if (m_overlayStartup->GetAlpha() == 0.0f && m_overlayStartup->isStarting())
	{
		m_overlayStartup->SetAlphaDriver(new driverconstant(0.0f));
		m_overlayDebug->SetAlphaDriver(new driverslide(0.0f, 1.0f, SLIDE_TIME));
		//	m_overlayManager->setAlphaDriver(new driverslide(0.0f, 1.0f, SLIDE_TIME));
		m_overlayStartup->setStarting(false);
	}
}

bool ManagerUis::IsGamePaused()
{
	return m_overlayStartup->isStarting();
}

float ManagerUis::GetBlurFactor()
{
	return 0.0f;
}
