#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>
#include <Uis/Inputs/UiInputButton.hpp>

using namespace acid;

namespace test
{
	class UiTab
	{
	private:
		std::string m_name;
		Colour m_colour;
		std::unique_ptr<UiInputButton> m_button;
		std::unique_ptr<UiObject> m_content;
	public:
		UiTab(const std::string &name, const Colour &colour, UiInputButton *button, UiObject *content) :
			m_name(name),
			m_colour(colour),
			m_button(button),
			m_content(content)
		{
		}

		std::string GetName() const { return m_name; }

		Colour GetColour() const { return m_colour; }

		UiInputButton *GetButton() const { return m_button.get(); }

		UiObject *GetContent() const { return m_content.get(); }
	};

	class UiNavigation :
		public UiObject
	{
	private:
		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Gui> m_navigation;
		std::unique_ptr<Text> m_title;
		std::unique_ptr<Text> m_createdBy;
		std::vector<std::unique_ptr<UiTab>> m_tabs;

		std::unique_ptr<IDriver<float>> m_driverTarget;
		UiTab *m_currentTab;
		UiTab *m_targetTab;
	public:
		static const Colour ColourPanel;
		static const Colour ColourButton;

		explicit UiNavigation(UiObject *parent);

		void UpdateObject() override;

		void SwitchTab(UiTab *tab);
	};
}
