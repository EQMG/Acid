#pragma once

#include <functional>

#include "../visual/driverslide.h"

#include "uis.h"
#include "uiobject.h"
#include "uialign.h"

namespace flounder
{
	class inputbutton :
		public uiobject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static colour *const COLOUR_NORMAL;

		text *m_text;
		gui *m_background;

		bool m_mouseOver;

		std::function<void()> m_actionLeft;
		std::function<void()> m_actionRight;
	public:
		inputbutton(uiobject *parent, const vector2 &position, const std::string &string, const uialign &align);

		~inputbutton();

		void updateObject() override;

		std::string getText() const { return m_text->getTextString(); }

		void setText(const std::string &string) const { m_text->setText(string); }

		void setActionLeft(std::function<void()> action) { m_actionLeft = action; }

		void setActionRight(std::function<void()> action) { m_actionRight = action; }
	};
}
