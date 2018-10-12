#pragma once

#include <Scenes/IScene.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/ButtonCompound.hpp>
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
		ButtonKeyboard m_buttonFullscreen;
		ButtonKeyboard m_buttonScreenshot;
		ButtonCompound m_buttonPause;
		ButtonKeyboard m_buttonExit;

		std::unique_ptr<UiStartLogo> m_uiStartLogo;
		std::unique_ptr<OverlayDebug> m_overlayDebug;
		std::unique_ptr<UiNavigation> m_uiNavigation;
	public:
		Scene1();

		void Start() override;

		void Update() override;

		bool IsPaused() const override;
	private:
		void TogglePause();
	};
}
