#pragma once

#include <Flounder.hpp>

using namespace Flounder;

namespace Demo
{
	class UiNavigation :
			public UiObject
	{
	private:
		UiObject *m_uiBar;
		Gui *m_barBackground;
		Text *m_barTitle;
		Text *m_barCreatedBy;
		Gui *m_tabPuck;
		std::vector<Text*> tabTexts;
	public:
		UiNavigation(UiObject *parent);

		~UiNavigation();

		virtual void UpdateObject() override;
	};
}
