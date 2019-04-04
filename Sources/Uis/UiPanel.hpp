#pragma once

#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiScrollBar.hpp"

namespace acid
{
class ACID_EXPORT UiPanel :
	public UiObject
{
public:
	explicit UiPanel(UiObject *parent, const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size),
		const Colour &colour = Colour::White, const BitMask<ScrollBar> &scrollBars = ScrollBar::Vertical | ScrollBar::Horizontal);

	void UpdateObject() override;

	UiObject &GetContent() { return m_content; }

	const BitMask<ScrollBar> &GetScrollBars() const { return m_scrollBars; }

	void SetScrollBars(const BitMask<ScrollBar> &scrollBars) { m_scrollBars = scrollBars; }

private:
	void SetScissor(UiObject *object, const bool &checkSize = false);

	Gui m_background;
	UiObject m_content;
	UiScrollBar m_scrollX;
	UiScrollBar m_scrollY;

	BitMask<ScrollBar> m_scrollBars;
	Vector2f m_min;
	Vector2f m_max;
};
}
