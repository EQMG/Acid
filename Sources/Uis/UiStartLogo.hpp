#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"

namespace acid
{
	class ACID_EXPORT UiStartLogo :
		public UiObject
	{
	private:
		Gui *m_guiBackground;
		Gui *m_guiLogo;
		Text *m_textCopyright;
		bool m_starting;
	public:
		UiStartLogo(UiObject *parent);

		~UiStartLogo();

		void UpdateObject() override;

		bool IsStarting() const { return m_starting; }

		void SetStarting(const bool &starting) { m_starting = starting; }
	};
}
