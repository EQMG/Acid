#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "Inputs/InputDelay.hpp"

namespace acid
{
	class ACID_EXPORT UiInputText :
		public UiObject
	{
	private:
		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		std::string m_prefix;
		std::string m_value;
		int32_t m_maxLength;

		InputDelay m_inputDelay;
		int32_t m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		Delegate<void(UiInputText *, std::string)> m_onType;
	public:
		UiInputText(UiObject *parent, const std::string &prefix, const std::string &value, const int32_t &maxLength,
			const UiBound &rectangle = UiBound(Vector3::Zero, UiBound::Centre, true, true, Vector2(0.36f, 0.05f)),
			const Colour &primaryColour = Colour("#171717"));

		void UpdateObject() override;

		const std::string &GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		const std::string &GetValue() const { return m_value; }

		void SetValue(const std::string &value);

		Delegate<void(UiInputText *, std::string)> &GetOnType() { return m_onType; }
	};
}
