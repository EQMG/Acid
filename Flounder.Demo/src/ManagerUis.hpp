#pragma once

#include <Flounder.hpp>

#include "uis/OverlayDebug.hpp"
#include "uis/OverlayPaused.hpp"

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

		OverlayStartup *m_overlayStartup;
		OverlayDebug *m_overlayDebug;
		OverlayPaused *m_overlayPaused;
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
