#pragma once

#include <functional>
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "UiObject.hpp"

namespace fl
{
	class FL_EXPORT UiInputButton :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static const Colour *COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;

		bool m_mouseOver;
	public:
		UiInputButton(UiObject *parent, const Vector2 &position, const std::string &string, const FontJustify &justify);

		~UiInputButton();

		void UpdateObject() override;

		std::string GetText() const { return m_text->GetText(); }

		void SetText(const std::string &string) const { m_text->SetText(string); }
	};
}
