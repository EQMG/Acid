#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Renderer/Renderer.hpp>
#include <Uis/Uis.hpp>
#include "FixedCamera.hpp"

namespace test
{
	const float UI_SLIDE_TIME = 0.2f;

	Scene1::Scene1() :
		IScene(new FixedCamera()),
		m_buttonFullscreen(std::make_unique<ButtonKeyboard>(std::vector<Key>{KEY_F11})),
		m_buttonScreenshot(std::make_unique<ButtonKeyboard>(std::vector<Key>{KEY_F12})),
		m_buttonPause(std::make_unique<ButtonCompound>(std::vector<IButton *>{
			new ButtonKeyboard(std::vector<Key>{KEY_ESCAPE}),
			new ButtonJoystick(JOYSTICK_1, std::vector<uint32_t>{7})
		})),
		m_buttonExit(std::make_unique<ButtonKeyboard>(std::vector<Key>{KEY_DELETE})),
		m_primaryColour(Colour("#e74c3c")),
		m_selectorJoystick(SelectorJoystick(JOYSTICK_1, 0, 1, {0, 1})),
		m_uiStartLogo(std::make_unique<UiStartLogo>(Uis::Get()->GetContainer())),
		m_overlayDebug(std::make_unique<OverlayDebug>(Uis::Get()->GetContainer())),
		m_uiNavigation(std::make_unique<UiNavigation>(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant>(0.0f);
		m_uiNavigation->SetAlphaDriver<DriverConstant>(0.0f);
	}

	Scene1::~Scene1()
	{
	}

	void Scene1::Start()
	{
	}

	void Scene1::Update()
	{
		if (m_buttonFullscreen->WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonScreenshot->WasDown())
		{
			std::string filename = "Screenshots/" + Engine::Get()->GetDateTime() + ".png";
			Renderer::Get()->CaptureScreenshot(filename);
		}

		if (m_buttonExit->WasDown())
		{
			Engine::Get()->RequestClose(false);
		}

		if (m_buttonPause->WasDown())
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


	bool Scene1::IsGamePaused() const
	{
		return m_uiStartLogo->IsStarting() || m_uiNavigation->GetAlpha() != 0.0f;
	}

	void Scene1::TogglePause()
	{
		if (m_uiStartLogo->IsStarting())
		{
			return;
		}

		if (IsGamePaused())
		{
			m_uiNavigation->SetAlphaDriver<DriverSlide>(m_uiNavigation->GetAlpha(), 0.0f, UI_SLIDE_TIME);
		}
		else
		{
			m_uiNavigation->SetAlphaDriver<DriverSlide>(m_uiNavigation->GetAlpha(), 1.0f, UI_SLIDE_TIME);
		}
	}
}
