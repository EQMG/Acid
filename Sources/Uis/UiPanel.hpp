#pragma once

#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiScrollBar.hpp"
#include "Inputs/UiInputButton.hpp"

namespace acid
{
class ACID_EXPORT UiPanel :
	public UiObject
{
public:
	enum class Resize
	{
		None, Left, Top, Right, Bottom
	};

	explicit UiPanel(UiObject *parent, const UiTransform &transform = UiTransform(Vector2i(), Vector2i(), UiAnchor::Centre),
		const Colour &colour = UiInputButton::BackgroundColour, const Resize &resize = Resize::None, const BitMask<ScrollBar> &scrollBars = ScrollBar::Vertical | ScrollBar::Horizontal);

	void UpdateObject() override;

	UiObject &GetContent() { return m_content; }

	const BitMask<ScrollBar> &GetScrollBars() const { return m_scrollBars; }

	void SetScrollBars(const BitMask<ScrollBar> &scrollBars) { m_scrollBars = scrollBars; }

private:
	void SetScissor(UiObject *object, const bool &checkSize = false);

	Gui m_background;
	UiObject m_content;

	UiObject m_resizeHandle;
	Resize m_resize;

	//UiScrollBar m_scrollX;
	//UiScrollBar m_scrollY;
	BitMask<ScrollBar> m_scrollBars;

	Vector2f m_min;
	Vector2f m_max;
};
}
