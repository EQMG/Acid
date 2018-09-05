#pragma once

#include <Audio/Sound.hpp>
#include <Inputs/IButton.hpp>
#include <Scenes/IScene.hpp>
#include <Uis/UiStartLogo.hpp>
#include "Uis/OverlayDebug.hpp"

using namespace acid;

namespace test
{
	class Scene1 :
		public IScene
	{
	private:
		std::shared_ptr<IButton> m_buttonSpawnSphere;
		std::shared_ptr<IButton> m_buttonFullscreen;
		std::shared_ptr<IButton> m_buttonCaptureMouse;
		std::shared_ptr<IButton> m_buttonScreenshot;
		std::shared_ptr<IButton> m_buttonExit;
		Sound m_soundScreenshot;

		Colour m_primaryColour;
		SelectorJoystick m_selectorJoystick;

		UiStartLogo *m_uiStartLogo;
		OverlayDebug *m_overlayDebug;
	public:
		Scene1();

		~Scene1();

		void Start() override;

		void Update() override;

		bool IsGamePaused() const override;

		Colour GetUiColour() const override { return m_primaryColour; }

		SelectorJoystick GetSelectorJoystick() const override { return m_selectorJoystick; };
	};
}
