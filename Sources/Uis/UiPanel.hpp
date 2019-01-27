#pragma once

#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	enum class ScrollBar
	{
		None = 0,
		Vertical = 1,
		Horizontal = 2
	};
	ENABLE_BITMASK_OPERATORS(ScrollBar)

	class ACID_EXPORT UiPanel :
		public UiObject
	{
	public:
		UiPanel(UiObject *parent, const UiBound &rectangle = UiBound(Vector3::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions),
			const bitmask<ScrollBar> &scrollBars = ScrollBar::Vertical | ScrollBar::Horizontal, const Colour &primaryColour = Colour("#161618"), const Colour &secondaryColour = Colour("#53575f", 0.85f));

		virtual void UpdateObject() override;

		UiObject *GetContent() const { return m_content.get(); }

		const bitmask<ScrollBar> &GetScrollBars() const { return m_scrollBars; }

		void SetScrollBars(const bitmask<ScrollBar> &scrollBars) { m_scrollBars = scrollBars; }
	private:
		void SetScissor(UiObject *object, const bool &size = false);

		static float ScrollByDelta(const float &delta, Gui *puck, UiObject *background, const uint32_t &index);

		static float ScrollByPosition(const float &position, Gui *puck, UiObject *background, const uint32_t &index);

		std::unique_ptr<Gui> m_background;
		std::unique_ptr<UiObject> m_content;
		std::unique_ptr<Gui> m_scrollX;
		std::unique_ptr<Gui> m_scrollY;

		bitmask<ScrollBar> m_scrollBars;
		bool m_updatingX;
		bool m_updatingY;
		Vector2 m_min;
		Vector2 m_max;
	};
}
