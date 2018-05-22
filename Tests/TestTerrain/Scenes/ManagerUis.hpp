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
	public:
		static const float SLIDE_TIME;
	private:
		Colour *m_primaryColour;

		UiStartLogo *m_uiStartLogo;
		OverlayDebug *m_overlayDebug;
	public:
		ManagerUis();

		~ManagerUis();

		void Update() override;

		bool IsGamePaused() override;

		float GetBlurFactor() override;

		Colour *GetPrimaryColour() override { return m_primaryColour; }
	};
}
