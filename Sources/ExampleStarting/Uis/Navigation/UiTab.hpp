#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>

using namespace Flounder;

namespace Demo
{
	class UiTab :
		public UiObject
	{
	private:
		std::string m_name;
		Colour m_colour;
		Text* m_text;
		float m_width;
	public:
		UiTab(UiObject *parent, const UiBound &rectangle, const std::string& name, const Colour& colour);

		~UiTab();

		void UpdateObject() override;

		std::string GetName() const { return m_name; }

		Colour GetColour() const { return m_colour; }

		float GetWidth() const { return m_width; }
	};
}
