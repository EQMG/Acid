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

		bool IsStarting() const { return GetAlpha() > 0.0f; }
	private:
		std::unique_ptr<Gui> m_guiBackground;
		std::unique_ptr<Gui> m_guiLogoAcid;
		std::unique_ptr<Text> m_textCopyright;
	};
}
