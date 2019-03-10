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
		UiPanel(UiObject *parent, const UiBound &rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions), 
			const Bitmask<ScrollBar> &scrollBars = ScrollBar::Vertical | ScrollBar::Horizontal);

		void UpdateObject() override;

		UiObject &GetContent() { return m_content; }

		const Bitmask<ScrollBar> &GetScrollBars() const { return m_scrollBars; }

		void SetScrollBars(const Bitmask<ScrollBar> &scrollBars) { m_scrollBars = scrollBars; }
	private:
		void SetScissor(UiObject *object, const bool &size = false);

		Gui m_background;
		UiObject m_content;
		UiScrollBar m_scrollX;
		UiScrollBar m_scrollY;

		Bitmask<ScrollBar> m_scrollBars;
		Vector2 m_min;
		Vector2 m_max;
	};
}
