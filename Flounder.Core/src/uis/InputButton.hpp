#pragma once

#include <functional>
#include "../fonts/Justify.hpp"
#include "Uis.hpp"
#include "UiObject.hpp"

namespace Flounder
{
	class InputButton :
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
		InputButton(UiObject *parent, const Vector2 &position, const std::string &string, const Justify &justify);

		~InputButton();

		void UpdateObject() override;

		std::string GetText() const { return m_text->GetText(); }

		void SetText(const std::string &string) const { m_text->SetText(string); }

		void SetActionLeft(std::function<void()> action) { m_actionLeft = action; }

		void SetActionRight(std::function<void()> action) { m_actionRight = action; }
	};
}
