#include "Scene1.hpp"

#include <thread>
#include <Animations/MeshAnimated.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Renderer/Renderer.hpp>
#include <Uis/Uis.hpp>

namespace test
{
	const Time UI_SLIDE_TIME = Time::Seconds(0.2f);

	Scene1::Scene1() :
		Scene(new Camera(), new VirtualJoystick(JOYSTICK_1, 0, 1, {0, 1})),
		m_buttonFullscreen(ButtonKeyboard(KEY_F11)),
		m_buttonScreenshot(ButtonKeyboard(KEY_F12)),
		m_buttonPause(ButtonCompound({
			new ButtonKeyboard(KEY_ESCAPE),
			new ButtonJoystick(JOYSTICK_1, 7)
		})),
		m_buttonExit(ButtonKeyboard(KEY_DELETE)),
		m_uiStartLogo(std::make_unique<UiStartLogo>(Uis::Get()->GetContainer())),
		m_overlayDebug(std::make_unique<OverlayDebug>(Uis::Get()->GetContainer())),
		m_uiNavigation(std::make_unique<UiNavigation>(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant>(0.0f);
		m_uiNavigation->SetAlphaDriver<DriverConstant>(0.0f);
	}

	void Scene1::Start()
	{
	}

	void Scene1::Update()
	{
		if (m_buttonFullscreen.WasDown())
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}

		if (m_buttonScreenshot.WasDown())
		{
			// TODO: Threading.
			std::thread t([](){
				std::string filename = "Screenshots/" + Engine::GetDateTime() + ".png";
				Renderer::Get()->CaptureScreenshot(filename);
			});
			t.detach();
		}

		if (m_buttonExit.WasDown())
		{
			Engine::Get()->RequestClose(false);
		}

		if (m_buttonPause.WasDown())
		{
			TogglePause();
		}

		if (m_uiStartLogo->GetAlpha() == 0.0f && m_uiStartLogo->IsStarting())
		{
			m_uiStartLogo->SetAlphaDriver<DriverConstant>(0.0f);
			m_overlayDebug->SetAlphaDriver<DriverSlide>(0.0f, 1.0f, UI_SLIDE_TIME);
		//  m_uiNavigation->SetAlphaDriver<DriverSlide>(0.0f, 1.0f, SLIDE_TIME);
			m_uiStartLogo->SetStarting(false);
			TogglePause();
		}
	}

	bool Scene1::IsPaused() const
	{
		return m_uiStartLogo->IsStarting() || m_uiNavigation->GetAlpha() != 0.0f;
	}

	void Scene1::TogglePause()
	{
		if (m_uiStartLogo->IsStarting())
		{
			return;
		}

		if (IsPaused())
		{
			m_uiNavigation->SetAlphaDriver<DriverSlide>(m_uiNavigation->GetAlpha(), 0.0f, UI_SLIDE_TIME);
		}
		else
		{
			m_uiNavigation->SetAlphaDriver<DriverSlide>(m_uiNavigation->GetAlpha(), 1.0f, UI_SLIDE_TIME);
		}
	}
}
