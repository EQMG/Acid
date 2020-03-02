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

	const std::string &GetTitle() const { return title.GetString(); }
	void SetTitle(const std::string &string) { title.SetString(string); }

	/**
	 * Called when this section has been collapsed or uncollapsed.
	 * @return The delegate.
	 */
	Delegate<void(UiSection *, bool)> &OnCollapsed() { return onCollapsed; }

private:
	Gui icon;
	Text title;
	UiObject content;

	bool collapsed = false;

	Delegate<void(UiSection *, bool)> onCollapsed;
};
}
