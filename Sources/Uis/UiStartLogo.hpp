#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"

namespace acid {
class ACID_EXPORT UiStartLogo : public UiObject {
public:
	explicit UiStartLogo();

	void UpdateObject() override;

	bool IsFinished() const { return finished; }
	Delegate<void()> &OnFinished() { return onFinished; }

#if defined(ACID_DEBUG)
	static constexpr Time StartDelay = 1s;
#else
	static constexpr Time StartDelay = 3s;
#endif
	
private:
	Gui background;
	Gui logoAcid;
	Text textCopyright;

	bool finished = false;

	Delegate<void()> onFinished;
};
}
