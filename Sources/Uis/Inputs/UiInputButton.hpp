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
	public:
		UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle = UiBound(Vector3::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions, Vector2(0.36f, 0.05f)),
			const Colour &primaryColour = Colour("#171717"));

		void UpdateObject() override;

		const std::string &GetString() const { return m_text->GetString(); }

		void SetString(const std::string &string) const { m_text->SetString(string); }

		Delegate<void(UiInputButton *, bool)> &GetOnPressed() { return m_onPressed; }
	private:
		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		bool m_mouseOver;

		Delegate<void(UiInputButton *, bool)> m_onPressed;
	};
}
