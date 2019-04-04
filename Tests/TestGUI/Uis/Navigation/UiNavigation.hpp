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

private:
	std::string m_name;
	Colour m_colour;
	std::unique_ptr<UiInputButton> m_button;
	std::unique_ptr<UiObject> m_content;
};

class UiNavigation :
	public UiObject
{
public:
	static const Colour ColourPanel;
	static const Colour ColourButton;

	explicit UiNavigation(UiObject *parent);

	void UpdateObject() override;

	void SwitchTab(UiTab *tab);

private:
	Gui m_background;
	Gui m_navigation;
	Text m_title;
	Text m_createdBy;
	std::vector<std::unique_ptr<UiTab>> m_tabs;

	std::unique_ptr<IDriver<float>> m_driverTarget;
	UiTab *m_currentTab;
	UiTab *m_targetTab;
};
}
