#pragma once

#include <Uis/UiStartLogo.hpp>
#include <Scenes/IManagerUis.hpp>
#include "Uis/OverlayDebug.hpp"

using namespace fl;

namespace test
{
	class ManagerUis :
		public IManagerUis
	{
	private:
		Colour *m_primaryColour;
		SelectorJoystick *m_selectorJoystick;

		UiStartLogo *m_uiStartLogo;
		OverlayDebug *m_overlayDebug;
	public:
		static const float SLIDE_TIME;

		ManagerUis();

		~ManagerUis();

		void Update() override;

		bool IsGamePaused() override;

		float GetBlurFactor() override;

		Colour *GetPrimaryColour() const override { return m_primaryColour; }

		SelectorJoystick *GetSelectorJoystick() const override { return m_selectorJoystick; };
	};
}
