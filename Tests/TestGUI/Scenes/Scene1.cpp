#include "Scene1.hpp"

#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Uis/Uis.hpp>

namespace test
{
const Time UI_SLIDE_TIME = Time::Seconds(0.2f);

Scene1::Scene1() :
	Scene(new Camera()),
	m_buttonPause(ButtonCompound({ new ButtonKeyboard(Key::Escape), new ButtonJoystick(0, 7) })),
	m_uiStartLogo(&Uis::Get()->GetCanvas()),
	m_overlayDebug(&Uis::Get()->GetCanvas()),
	m_uiPanels(&Uis::Get()->GetCanvas())
{
	m_buttonPause.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			TogglePause();
		}
	});

	m_uiStartLogo.SetAlphaDriver(new DriverConstant<float>(1.0f));
	m_overlayDebug.SetAlphaDriver(new DriverConstant<float>(0.0f));
	m_uiPanels.SetAlphaDriver(new DriverConstant<float>(0.0f));

	m_uiStartLogo.OnFinished().Add([this]()
	{
		m_overlayDebug.SetAlphaDriver(new DriverSlide<float>(0.0f, 1.0f, UI_SLIDE_TIME));
		//m_uiPanels.SetAlphaDriver(new DriverSlide<float>(0.0f, 1.0f, UI_SLIDE_TIME));
		TogglePause();
	});
}

void Scene1::Start()
{
}

void Scene1::Update()
{
}

bool Scene1::IsPaused() const
{
	return !m_uiStartLogo.IsFinished() || m_uiPanels.GetAlpha() > 0.0f;
}

void Scene1::TogglePause()
{
	if (!m_uiStartLogo.IsFinished())
	{
		return;
	}

	if (IsPaused())
	{
		m_uiPanels.SetAlphaDriver(new DriverSlide<float>(m_uiPanels.GetAlpha(), 0.0f, UI_SLIDE_TIME));
	}
	else
	{
		m_uiPanels.SetAlphaDriver(new DriverSlide<float>(m_uiPanels.GetAlpha(), 1.0f, UI_SLIDE_TIME));
	}
}
}
