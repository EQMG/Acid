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
		Scene(new Camera()),
		m_buttonFullscreen(ButtonKeyboard(Key::F11)),
		m_buttonScreenshot(ButtonKeyboard(Key::F12)),
		m_buttonPause(ButtonCompound({
			new ButtonKeyboard(Key::Escape),
			new ButtonJoystick(0, 7)
		})),
		m_buttonExit(ButtonKeyboard(Key::Delete)),
		m_uiStartLogo(std::make_unique<UiStartLogo>(Uis::Get()->GetContainer())),
		m_overlayDebug(std::make_unique<OverlayDebug>(Uis::Get()->GetContainer())),
		m_editorPanels(std::make_unique<EditorPanels>(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant<float>>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant<float>>(0.0f);
		m_editorPanels->SetAlphaDriver<DriverConstant<float>>(0.0f);
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
			m_uiStartLogo->SetAlphaDriver<DriverConstant<float>>(0.0f);
			m_overlayDebug->SetAlphaDriver<DriverSlide<float>>(0.0f, 1.0f, UI_SLIDE_TIME);
		//  m_uiNavigation->SetAlphaDriver<DriverSlide<float>>(0.0f, 1.0f, SLIDE_TIME);
			m_uiStartLogo->SetStarting(false);
			TogglePause();
		}
	}

	bool Scene1::IsPaused() const
	{
		return m_uiStartLogo->IsStarting() || m_editorPanels->GetAlpha() != 0.0f;
	}

	void Scene1::TogglePause()
	{
		if (m_uiStartLogo->IsStarting())
		{
			return;
		}

		if (IsPaused())
		{
			m_editorPanels->SetAlphaDriver<DriverSlide<float>>(m_editorPanels->GetAlpha(), 0.0f, UI_SLIDE_TIME);
		}
		else
		{
			m_editorPanels->SetAlphaDriver<DriverSlide<float>>(m_editorPanels->GetAlpha(), 1.0f, UI_SLIDE_TIME);
		}
	}
}
