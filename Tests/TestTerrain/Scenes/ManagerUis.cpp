#include "ManagerUis.hpp"

#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/Mouse.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>

namespace test
{
	const float ManagerUis::SLIDE_TIME = 0.2f;

	ManagerUis::ManagerUis() :
		IManagerUis(),
		m_primaryColour(new Colour("#e74c3c")),
		m_selectorJoystick(new SelectorJoystick(JoystickPort::JOYSTICK_1, 0, 1, 0, 1)),
		m_uiStartLogo(new UiStartLogo(Uis::Get()->GetContainer())),
		m_overlayDebug(new OverlayDebug(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver(new DriverConstant(1.0f));
		m_overlayDebug->SetAlphaDriver(new DriverConstant(0.0f));
	}

	ManagerUis::~ManagerUis()
	{
		delete m_primaryColour;
		delete m_selectorJoystick;
		delete m_uiStartLogo;
		delete m_overlayDebug;
	}

	void ManagerUis::Update()
	{
		if (m_uiStartLogo->GetAlpha() == 0.0f && m_uiStartLogo->IsStarting())
		{
			m_uiStartLogo->SetAlphaDriver(new DriverConstant(0.0f));
			m_overlayDebug->SetAlphaDriver(new DriverSlide(0.0f, 1.0f, SLIDE_TIME));
			m_uiStartLogo->SetStarting(false);
			Mouse::Get()->SetCursorHidden(true);
		}
	}

	bool ManagerUis::IsGamePaused()
	{
		return false;
	}

	float ManagerUis::GetBlurFactor()
	{
		return 0.0f;
	}
}
