#pragma once

#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiPanel :
		public UiObject
	{
	public:
		UiPanel(UiObject *parent, const UiBound &rectangle = UiBound(Vector3::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions),
			const Colour &primaryColour = Colour("#161618"), const Colour &secondaryColour = Colour("#3e4148"));

		virtual void UpdateObject() override;

		void ResetScrollbars();

		UiObject *GetContent() const { return m_content.get(); }
	private:
		void SetScissor(UiObject *object, const bool &size = false);

		static float ScrollByDelta(const float &delta, Gui *puck, Gui *scrollBar, const uint32_t &index);

		static float ScrollByPosition(const float &position, Gui *puck, Gui *scrollBar, const uint32_t &index);

		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Gui> m_scrollBarX;
		std::unique_ptr<Gui> m_puckX;
		std::unique_ptr<Gui> m_scrollBarY;
		std::unique_ptr<Gui> m_puckY;

		std::unique_ptr<UiObject> m_content;
		bool m_updatingX;
		bool m_updatingY;
		Vector2 m_min;
		Vector2 m_max;
	};
}
