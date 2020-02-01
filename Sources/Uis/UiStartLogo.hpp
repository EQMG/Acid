#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"

namespace acid {
class ACID_EXPORT UiStartLogo : public UiObject {
public:
	explicit UiStartLogo();

	void UpdateObject() override;

	bool IsFinished() const { return m_finished; }
	Delegate<void()> &OnFinished() { return m_onFinished; }

private:
	Gui m_guiBackground;
	Gui m_guiLogoAcid;
	Text m_textCopyright;

	bool m_finished = false;

	Delegate<void()> m_onFinished;
};
}
