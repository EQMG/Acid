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
	m_buttonPause(ButtonCompound({
		new ButtonKeyboard(Key::Escape),
		new ButtonJoystick(0, 7)
	})),
	m_uiStartLogo(&Uis::Get()->GetContainer()),
	m_overlayDebug(&Uis::Get()->GetContainer()),
	m_uiNavigation(&Uis::Get()->GetContainer())
{
	m_uiStartLogo.SetAlphaDriver(new DriverConstant<float>(1.0f));
	m_overlayDebug.SetAlphaDriver(new DriverConstant<float>(0.0f));
	m_uiNavigation.SetAlphaDriver(new DriverConstant<float>(0.0f));

	m_uiStartLogo.GetOnFinished() += [this]()
	{
		m_overlayDebug.SetAlphaDriver(new DriverSlide<float>(0.0f, 1.0f, UI_SLIDE_TIME));
		//  m_uiNavigation.SetAlphaDriver(new DriverSlide<float>(0.0f, 1.0f, UI_SLIDE_TIME));
		TogglePause();
	};
}

void Scene1::Start()
{
}

void Scene1::Update()
{
	if (m_buttonPause.WasDown())
	{
		TogglePause();
	}
}

bool Scene1::IsPaused() const
{
	return !m_uiStartLogo.IsFinished() || m_uiNavigation.GetAlpha() > 0.0f;
}

void Scene1::TogglePause()
{
	if (!m_uiStartLogo.IsFinished())
	{
		return;
	}

	if (IsPaused())
	{
		m_uiNavigation.SetAlphaDriver(new DriverSlide<float>(m_uiNavigation.GetAlpha(), 0.0f, UI_SLIDE_TIME));
	}
	else
	{
		m_uiNavigation.SetAlphaDriver(new DriverSlide<float>(m_uiNavigation.GetAlpha(), 1.0f, UI_SLIDE_TIME));
	}
}
}
