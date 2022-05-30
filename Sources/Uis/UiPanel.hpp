#pragma once

#include "Guis/Gui.hpp"
#include "Inputs/UiButtonInput.hpp"
#include "UiObject.hpp"
#include "UiScrollBar.hpp"

namespace acid {
enum class UiManipulate {
	None = 0,
	Resize = 1,
	Move = 2,
	All = Resize | Move
};
ENABLE_BITMASK_OPERATORS(UiManipulate)

class ACID_EXPORT UiPanel : public UiObject {
public:
	UiPanel();

	void UpdateObject() override;

	void AddChild(UiObject *child) override;

	void SetBackgroundColor(const Colour &colour);

	const bitmask::bitmask<UiManipulate> &GetManipulate() const { return manipulate; }
	void SetManipulate(const bitmask::bitmask<UiManipulate> &manipulate) { this->manipulate = manipulate; }
	
	const bitmask::bitmask<ScrollBar> &GetScrollBars() const { return scrollBars; }
	void SetScrollBars(const bitmask::bitmask<ScrollBar> &scrollBars) { this->scrollBars = scrollBars; }

private:
	void SetScissor(UiObject *object, bool checkSize = false);

	Gui background;
	UiObject content;

	Gui resizeHandle;
	bitmask::bitmask<UiManipulate> manipulate;

	UiScrollBar scrollX, scrollY;
	bitmask::bitmask<ScrollBar> scrollBars;

	Vector2f min, max;
};
}
