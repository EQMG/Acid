#include "ManagerUis.hpp"

#include <Inputs/ButtonKeyboard.hpp>
#include <Visual/DriverConstant.hpp>
#include <Visual/DriverSlide.hpp>

namespace Demo
{
	const float ManagerUis::SLIDE_TIME = 0.2f;

	ManagerUis::ManagerUis() :
		IManagerUis(),
		m_primaryColour(new Colour("#e74c3c")),
		m_buttonPause(new ButtonKeyboard({GLFW_KEY_ESCAPE})),
		m_uiStartLogo(new UiStartLogo(Uis::Get()->GetContainer())),
		m_overlayDebug(new OverlayDebug(Uis::Get()->GetContainer())),
		m_uiNavigation(new UiNavigation(Uis::Get()->GetContainer()))
	{
		Uis::Get()->GetSelector()->Load(0, 0, 1, 0, 1);
		m_uiStartLogo->SetAlphaDriver(new DriverConstant(1.0f));
		m_overlayDebug->SetAlphaDriver(new DriverConstant(0.0f));
		m_uiNavigation->SetAlphaDriver(new DriverConstant(0.0f));
	}

	ManagerUis::~ManagerUis()
	{
		delete m_primaryColour;
		delete m_buttonPause;
		delete m_uiStartLogo;
		delete m_overlayDebug;
		delete m_uiNavigation;
	}

	void ManagerUis::Update()
	{
		if (m_buttonPause->WasDown())
		{
			TogglePause();
		}

		if (m_uiStartLogo->GetAlpha() == 0.0f && m_uiStartLogo->IsStarting())
		{
			m_uiStartLogo->SetAlphaDriver(new DriverConstant(0.0f));
			m_overlayDebug->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, SLIDE_TIME));
			//m_uiNavigation->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, SLIDE_TIME));
			m_uiStartLogo->SetStarting(false);
		}
	}

	bool ManagerUis::IsGamePaused()
	{
		return m_uiStartLogo->IsStarting() || m_uiNavigation->GetAlpha() != 0.0f;
	}

	float ManagerUis::GetBlurFactor()
	{
		return 0.0f;
	}

	void ManagerUis::TogglePause()
	{
		if (m_uiStartLogo->IsStarting())
		{
			return;
		}

		if (IsGamePaused())
		{
			m_uiNavigation->SetAlphaDriver(new DriverSlide(m_uiNavigation->GetAlpha(), 0.0f, SLIDE_TIME));
		}
		else
		{
			m_uiNavigation->SetAlphaDriver(new DriverSlide(m_uiNavigation->GetAlpha(), 1.0f, SLIDE_TIME));
		}
	}
}
