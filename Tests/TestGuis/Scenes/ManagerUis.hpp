#pragma once

#include <Uis/UiStartLogo.hpp>
#include <Scenes/IManagerUis.hpp>
#include "Uis/OverlayDebug.hpp"
#include "Uis/Navigation/UiNavigation.hpp"

using namespace fl;

namespace test
{
	class ManagerUis :
		public IManagerUis
	{
	public:
		static const float SLIDE_TIME;
	private:
		Colour *m_primaryColour;
		SelectorJoystick *m_selectorJoystick;

		IButton *m_buttonPause;

		UiStartLogo *m_uiStartLogo;
		OverlayDebug *m_overlayDebug;
		UiNavigation *m_uiNavigation;
	public:
		ManagerUis();

		~ManagerUis();

		void Update() override;

		bool IsGamePaused() override;

		float GetBlurFactor() override;

		Colour *GetPrimaryColour() const override { return m_primaryColour; }

		SelectorJoystick *GetSelectorJoystick() const override { return m_selectorJoystick; };
	private:
		void TogglePause();
	};
}
