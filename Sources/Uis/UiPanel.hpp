#pragma once

#include "Guis/Gui.hpp"
#include "Inputs/UiInputButton.hpp"
#include "UiObject.hpp"
#include "UiScrollBar.hpp"

namespace acid {
enum class UiManipulate {
	None = 0,
	Resize = 1,
	Move = 2,
	All = Resize | Move
};

ENABLE_BITMASK_OPERATORS(UiManipulate);

class ACID_EXPORT UiPanel : public UiObject {
public:
	UiPanel();

	void UpdateObject() override;

	void AddChild(UiObject *child) override;

	void SetBackgroundColor(const Colour &colour);

	const BitMask<UiManipulate> &GetManipulate() const { return manipulate; }
	void SetManipulate(const BitMask<UiManipulate> &manipulate) { this->manipulate = manipulate; }
	
	const BitMask<ScrollBar> &GetScrollBars() const { return scrollBars; }
	void SetScrollBars(const BitMask<ScrollBar> &scrollBars) { this->scrollBars = scrollBars; }

private:
	void SetScissor(UiObject *object, bool checkSize = false);

	Gui background;
	UiObject content;

	Gui resizeHandle;
	BitMask<UiManipulate> manipulate;

	UiScrollBar scrollX, scrollY;
	BitMask<ScrollBar> scrollBars;

	Vector2f min, max;
};
}
