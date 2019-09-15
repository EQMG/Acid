#pragma once

#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiScrollBar.hpp"
#include "Inputs/UiInputButton.hpp"

namespace acid {
enum class UiManipulate {
	None = 0,
	Resize = 1,
	Move = 2,
	All = Resize | Move
};

ENABLE_BITMASK_OPERATORS(UiManipulate)

class ACID_EXPORT
UiPanel
:
public
UiObject
{
public:
	UiPanel(UiObject *parent, const UiTransform &transform, const Colour &colour = UiInputButton::BackgroundColour,
		const BitMask<UiManipulate> &manipulate = UiManipulate::None, const BitMask<ScrollBar> &scrollBars = ScrollBar::Vertical | ScrollBar::Horizontal);

	void UpdateObject() override;

	UiObject &GetContent() { return m_content; }

	const BitMask<ScrollBar> &GetScrollBars() const { return m_scrollBars; }

	void SetScrollBars(const BitMask<ScrollBar> &scrollBars) { m_scrollBars = scrollBars; }

private:
	void SetScissor(UiObject *object, bool checkSize = false);

	Gui m_background;
	UiObject m_content;

	Gui m_resizeHandle;
	BitMask<UiManipulate> m_manipulate;

	UiScrollBar m_scrollX;
	UiScrollBar m_scrollY;
	BitMask<ScrollBar> m_scrollBars;

	Vector2f m_min;
	Vector2f m_max;
};
}
