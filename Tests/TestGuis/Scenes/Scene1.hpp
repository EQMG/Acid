#pragma once

#include <Inputs/IButton.hpp>
#include <Scenes/IScene.hpp>
#include <Uis/UiStartLogo.hpp>
#include "Uis/OverlayDebug.hpp"
#include "Uis/Navigation/UiNavigation.hpp"

using namespace acid;

namespace test
{
	class Scene1 :
		public IScene
	{
	private:
		IButton *m_buttonFullscreen;
		IButton *m_buttonScreenshot;
		IButton *m_buttonPause;
		IButton *m_buttonExit;

		Colour m_primaryColour;
		SelectorJoystick m_selectorJoystick;
		
		UiStartLogo *m_uiStartLogo;
		OverlayDebug *m_overlayDebug;
		UiNavigation *m_uiNavigation;
	public:
		Scene1();

		~Scene1();

		void Start() override;

		void Update() override;

		bool IsGamePaused() const override;

		Colour GetUiColour() const override { return m_primaryColour; }

		SelectorJoystick GetSelectorJoystick() const override { return m_selectorJoystick; };
	private:
		void TogglePause();
	};
}
