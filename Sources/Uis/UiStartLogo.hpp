#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"

namespace acid
{
	class ACID_EXPORT UiStartLogo :
		public UiObject
	{
	public:
		explicit UiStartLogo(UiObject *parent);

		void UpdateObject() override;

		void Start();

		const bool &IsStarting() const { return m_starting; }
	private:
		std::unique_ptr<Gui> m_guiBackground;
		std::unique_ptr<Gui> m_guiLogoAcid;
		std::unique_ptr<Text> m_textCopyright;
		bool m_starting;
	};
}
