#pragma once

#include <Uis/UiStartLogo.hpp>
#include "Uis/OverlayDebug.hpp"
#include "Uis/Navigation/UiNavigation.hpp"

using namespace Flounder;

namespace Demo
{
	class ManagerUis :
		public IManagerUis
	{
	public:
		static const float SLIDE_TIME;
	private:
		Colour *m_primaryColour;

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

		inline Colour *GetPrimaryColour() override { return m_primaryColour; }
	private:
		void TogglePause();
	};
}
