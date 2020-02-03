#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid {
class ACID_EXPORT UiSection : public UiObject {
public:
	UiSection();

	void UpdateObject() override;

	void AddChild(UiObject *child) override;

	const std::string &GetTitle() const { return m_title.GetString(); }
	void SetTitle(const std::string &string) { m_title.SetString(string); }

	/**
	 * Called when this section has been collapsed or uncollapsed.
	 * @return The delegate.
	 */
	Delegate<void(UiSection *, bool)> &OnCollapsed() { return m_onCollapsed; }

private:
	Gui m_icon;
	Text m_title;
	UiObject m_content;

	bool m_collapsed = false;

	Delegate<void(UiSection *, bool)> m_onCollapsed;
};
}
