#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid {
class ACID_EXPORT UiSection : public UiObject {
public:
	explicit UiSection(const std::string &string);

	void UpdateObject() override;

	void SetTransform(const UiTransform &transform) override;

	UiObject &GetContent() { return m_content; }

	/**
	 * Called when this section has been collapsed or uncollapsed.
	 * @return The delegate.
	 */
	Delegate<void(UiSection *, bool)> &OnCollapsed() { return m_onCollapsed; }

private:
	Gui m_icon;
	Text m_text;
	UiObject m_content;

	bool m_collapsed = false;

	Delegate<void(UiSection *, bool)> m_onCollapsed;
};
}
