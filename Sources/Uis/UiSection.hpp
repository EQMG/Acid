#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiSection :
		public UiObject
	{
	public:
		UiSection(UiObject *parent, const std::string &string,
			const UiBound &rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

		void UpdateObject() override;

		UiObject *GetContent() const { return m_content.get(); }

		Delegate<void(UiSection *, bool)> &GetOnCollapsed() { return m_onCollapsed; }
	private:
		std::unique_ptr<Gui> m_icon;
		std::unique_ptr<Text> m_text;
		std::unique_ptr<UiObject> m_content;
		Sound m_soundClick;

		bool m_collapsed;
		bool m_mouseOver;

		Delegate<void(UiSection *, bool)> m_onCollapsed;
	};
}
