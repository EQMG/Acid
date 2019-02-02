#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Renderer/Renderer.hpp>
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
		m_uiStartLogo(std::make_unique<UiStartLogo>(Uis::Get()->GetContainer())),
		m_overlayDebug(std::make_unique<OverlayDebug>(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant<float>>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant<float>>(0.0f);
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

		if (m_uiStartLogo != nullptr && !m_uiStartLogo->IsStarting())
		{
			m_overlayDebug->SetAlphaDriver<DriverSlide<float>>(0.0f, 1.0f, UI_SLIDE_TIME);
			m_uiStartLogo = nullptr;
			TogglePause();
		}
	}

	bool Scene1::IsPaused() const
	{
		return m_uiStartLogo != nullptr;
	}

	void Scene1::TogglePause()
	{
		if (m_uiStartLogo != nullptr)
		{
			return;
		}

		if (IsPaused())
		{
		//	m_uis->SetAlphaDriver<DriverSlide<float>>(m_editorPanels->GetAlpha(), 0.0f, UI_SLIDE_TIME);
		}
		else
		{
		//	m_uis->SetAlphaDriver<DriverSlide<float>>(m_editorPanels->GetAlpha(), 1.0f, UI_SLIDE_TIME);
		}
	}
}
