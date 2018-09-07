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
		std::unique_ptr<Gui> m_guiLogo;
		std::unique_ptr<Text> m_textCopyright;
		bool m_starting;
	public:
		UiStartLogo(UiObject *parent);

		~UiStartLogo();

		void UpdateObject() override;

		bool IsStarting() const { return m_starting; }

		void SetStarting(const bool &starting) { m_starting = starting; }
	};
}
