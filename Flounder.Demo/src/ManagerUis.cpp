#include "ManagerUis.hpp"

namespace Demo
{
	const float ManagerUis::SLIDE_TIME = 0.5f;

	ManagerUis::ManagerUis() :
		IManagerUis(),
		m_primaryColour(new Colour("#553982")),
		m_overlayStartup(new OverlayStartup(Uis::Get()->GetContainer())),
		m_overlayDebug(new OverlayDebug(Uis::Get()->GetContainer())),
		m_overlayManager(new OverlayManager(Uis::Get()->GetContainer()))
	{
		Uis::Get()->GetSelector()->Load(0, 0, 1, 0, 1);

		m_overlayStartup->SetAlphaDriver(new DriverConstant(1.0f));
		m_overlayDebug->SetAlphaDriver(new DriverConstant(0.0f));
		m_overlayManager->SetAlphaDriver(new DriverConstant(0.0f));
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
		if (m_overlayStartup->GetAlpha() == 0.0f && m_overlayStartup->IsStarting())
		{
			m_overlayStartup->SetAlphaDriver(new DriverConstant(0.0f));
		//	m_overlayDebug->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, SLIDE_TIME));
		//	m_overlayManager->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, SLIDE_TIME)); // For testing.
			m_overlayStartup->SetStarting(false);
		}
	}

	bool ManagerUis::IsGamePaused()
	{
		return m_overlayStartup->IsStarting();
	}

	float ManagerUis::GetBlurFactor()
	{
		return 0.0f;
	}
}
