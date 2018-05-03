#pragma once

#include <Uis/UiObject.hpp>
#include <Uis/UiInputButton.hpp>
#include <Guis/Gui.hpp>

using namespace fl;

namespace Demo
{
	class ContentExit :
		public UiObject
	{
	private:
		Gui *m_background;
		UiInputButton *m_button1;
	public:
		ContentExit(UiObject *parent);

		~ContentExit();

		void UpdateObject() override;

	};
}
