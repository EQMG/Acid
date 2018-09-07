#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>
#include "UiTab.hpp"

using namespace acid;

namespace test
{
	class UiNavigation :
		public UiObject
	{
	private:
		std::unique_ptr<Gui> m_barBackground;
		std::unique_ptr<Text> m_barTitle;
		std::unique_ptr<Text> m_barCreatedBy;
		std::unique_ptr<Gui> m_tabPuck;
		std::vector<std::unique_ptr<UiTab>> m_tabs;

		std::unique_ptr<IDriver> m_driverTarget;
		UiTab *m_currentTab;
		UiTab *m_targetTab;
	public:
		UiNavigation(UiObject *parent);

		~UiNavigation();

		void UpdateObject() override;

		void SwitchTab(const std::string &tabName);
	};
}
