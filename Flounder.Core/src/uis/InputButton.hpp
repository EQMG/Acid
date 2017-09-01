#pragma once

#include <functional>

#include "../visual/driverslide.hpp"

#include "uis.hpp"
#include "uiobject.hpp"
#include "uialign.hpp"

namespace Flounder
{
	class inputbutton :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static Colour *const COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;

		bool m_mouseOver;

		std::function<void()> m_actionLeft;
		std::function<void()> m_actionRight;
	public:
		inputbutton(UiObject *parent, const Vector2 &position, const std::string &string, const uialign &align);

		~inputbutton();

		void UpdateObject() override;

		std::string getText() const { return m_text->GetText(); }

		void setText(const std::string &string) const { m_text->setText(string); }

		void setActionLeft(std::function<void()> action) { m_actionLeft = action; }

		void setActionRight(std::function<void()> action) { m_actionRight = action; }
	};
}
