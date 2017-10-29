#include "ManagerUis.hpp"

namespace Demo
{
	const float ManagerUis::SLIDE_TIME = 0.5f;

	ManagerUis::ManagerUis() :
		IManagerUis(),
		m_primaryColour(new Colour("#e74c3c")),
		m_buttonPause(new ButtonKeyboard({ GLFW_KEY_ESCAPE })),
		m_overlayStartup(new OverlayStartup(Uis::Get()->GetContainer())),
		m_overlayDebug(new OverlayDebug(Uis::Get()->GetContainer())),
		m_overlayPaused(new OverlayPaused(Uis::Get()->GetContainer()))
	{
		Uis::Get()->GetSelector()->Load(0, 0, 1, 0, 1);

		m_overlayStartup->SetAlphaDriver(new DriverConstant(1.0f));
		m_overlayDebug->SetAlphaDriver(new DriverConstant(0.0f)); 
		m_overlayPaused->SetAlphaDriver(new DriverConstant(0.0f));
	}

	ManagerUis::~ManagerUis()
	{
		delete m_primaryColour;
		delete m_buttonPause;
		delete m_overlayStartup;
		delete m_overlayDebug;
		delete m_overlayPaused;
	}

	void ManagerUis::Update()
	{
		if (m_buttonPause->WasDown())
		{
			TogglePause();
		}

		if (m_overlayStartup->GetAlpha() == 0.0f && m_overlayStartup->IsStarting())
		{
			m_overlayStartup->SetAlphaDriver(new DriverConstant(0.0f));
			m_overlayDebug->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, SLIDE_TIME));
		//	m_overlayPaused->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, SLIDE_TIME));
			m_overlayStartup->SetStarting(false);
		}
	}

	bool ManagerUis::IsGamePaused()
	{
		return m_overlayStartup->IsStarting() || m_overlayPaused->GetAlpha() != 0.0f;
	}

	float ManagerUis::GetBlurFactor()
	{
		return 0.0f;
	}

	void ManagerUis::TogglePause()
	{
		if (m_overlayStartup->IsStarting())
		{
			return;
		}

		if (IsGamePaused())
		{
			m_overlayPaused->SetAlphaDriver(new DriverSlide(m_overlayPaused->GetAlpha(), 0.0f, SLIDE_TIME));
		}
		else 
		{
			m_overlayPaused->SetAlphaDriver(new DriverSlide(m_overlayPaused->GetAlpha(), 1.0f, SLIDE_TIME));
		}
	}
}
