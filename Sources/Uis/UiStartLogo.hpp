#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"

namespace acid
{
	class ACID_EXPORT UiStartLogo :
		public UiObject
	{
	private:
		std::unique_ptr<Gui> m_guiBackground;
		std::unique_ptr<Gui> m_guiLogoAcid;
		std::unique_ptr<Text> m_textCopyright;
		bool m_starting;
	public:
		explicit UiStartLogo(UiObject *parent);

		void UpdateObject() override;

		bool IsStarting() const { return m_starting; }

		void SetStarting(const bool &starting) { m_starting = starting; }
	};
}
