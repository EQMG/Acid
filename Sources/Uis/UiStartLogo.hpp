﻿#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"

namespace acid
{
class UiStartLogo :
	public UiObject
{
public:
	explicit UiStartLogo(UiObject *parent);

	void UpdateObject() override;

	bool IsFinished() const { return m_finished; }

	Delegate<void()> &OnFinished() { return m_onFinished; }

private:
	Gui m_guiBackground;
	Gui m_guiLogoAcid;
	Text m_textCopyright;

	bool m_finished;

	Delegate<void()> m_onFinished;
};
}
