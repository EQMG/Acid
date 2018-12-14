#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiInputButton :
		public UiObject
	{
	private:
		static const Time CHANGE_TIME;
		static const float FONT_SIZE;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;

		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		bool m_mouseOver;
	public:
		UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle = UiBound(Vector3::ZERO, UiBound::CENTRE, true, true, Vector2(0.36f, 0.05f)),
			const Colour &primaryColour = Colour("#171717"));

		void UpdateObject() override;

		Gui *GetBackground() const { return m_background.get(); }

		Text *GetText() const { return m_text.get(); }

		std::string GetString() const { return m_text->GetString(); }

		void SetString(const std::string &string) const { m_text->SetString(string); }
	};
}
