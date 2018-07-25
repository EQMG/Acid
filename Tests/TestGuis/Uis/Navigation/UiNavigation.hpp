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
		Gui *m_barBackground;
		Text *m_barTitle;
		Text *m_barCreatedBy;
		Gui *m_tabPuck;
		std::vector<UiTab *> m_tabs;

		IDriver *m_driverTarget;
		UiTab *m_currentTab;
		UiTab *m_targetTab;
	public:
		UiNavigation(UiObject *parent);

		~UiNavigation();

		void UpdateObject() override;

		void SwitchTab(const std::string &tabName);
	};
}
