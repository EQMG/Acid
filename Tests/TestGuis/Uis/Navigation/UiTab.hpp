#pragma once

#include <Audio/Sound.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>
#include <Uis/UiObject.hpp>

using namespace acid;

namespace test
{
	class UiTab :
		public UiObject
	{
	private:
		std::unique_ptr<UiObject> m_content;
		std::string m_name;
		Colour m_colour;
		std::unique_ptr<Text> m_text;
		float m_width;

		Sound m_soundClick;
	public:
		UiTab(UiObject *parent, UiObject *content, const UiBound &rectangle, const std::string &name, const Colour &colour);

		~UiTab();

		void UpdateObject() override;

		UiObject *GetContent() const { return m_content.get(); }

		std::string GetName() const { return m_name; }

		Colour GetColour() const { return m_colour; }

		float GetWidth() const { return m_width; }
	};
}
