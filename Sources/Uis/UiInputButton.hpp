#pragma once

#include <functional>
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiInputButton :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float FONT_SIZE;
		static const Vector2 DIMENSION;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;

		std::unique_ptr<Text> m_text;
		std::unique_ptr<Gui> m_background;

		bool m_mouseOver;
	public:
		UiInputButton(UiObject *parent, const Vector2 &position, const std::string &string);

		~UiInputButton();

		void UpdateObject() override;

		bool OnActionMouse(const MouseButton &button) override;

		std::string GetText() const { return m_text->GetString(); }

		void SetText(const std::string &string) const { m_text->SetString(string); }
	};
}
