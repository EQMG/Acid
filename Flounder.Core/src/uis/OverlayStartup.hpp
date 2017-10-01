#pragma once

#include "Uis.hpp"

namespace Flounder
{
	class OverlayStartup :
		public UiObject
	{
	private:
		Gui *m_guiBackground;
		Gui *m_guiLogo;
		Text *m_textCopyright;
		bool m_starting;
	public:
		OverlayStartup(UiObject *parent);

		~OverlayStartup();

		void UpdateObject() override;

		bool IsStarting() const { return m_starting; }

		void SetStarting(const bool &starting) { m_starting = starting; }
	};
}
