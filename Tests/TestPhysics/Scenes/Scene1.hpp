#pragma once

#include <Audio/Sound.hpp>
#include <Inputs/IButton.hpp>
#include <Inputs/ButtonMouse.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Scenes/Scene.hpp>
#include <Uis/UiStartLogo.hpp>
#include "Uis/OverlayDebug.hpp"

using namespace acid;

namespace test
{
	class Scene1 :
		public Scene
	{
	private:
		ButtonMouse m_buttonSpawnSphere;
		ButtonKeyboard m_buttonFullscreen;
		ButtonKeyboard m_buttonCaptureMouse;
		ButtonKeyboard m_buttonScreenshot;
		ButtonKeyboard m_buttonSave;
		ButtonKeyboard m_buttonExit;
		Sound m_soundScreenshot;

		std::unique_ptr<UiStartLogo> m_uiStartLogo;
		std::unique_ptr<OverlayDebug> m_overlayDebug;
	public:
		Scene1();

		void Start() override;

		void Update() override;

		bool IsPaused() const override;
	};
}
