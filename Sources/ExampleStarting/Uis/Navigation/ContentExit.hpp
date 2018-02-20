#pragma once

#include <Uis/UiObject.hpp>
#include <Uis/InputButton.hpp>
#include <Guis/Gui.hpp>

using namespace Flounder;

namespace Demo
{
	class ContentExit :
		public UiObject
	{
	private:
		Gui *m_background;
		InputButton *m_button1;
	public:
		ContentExit(UiObject *parent);

		~ContentExit();

		void UpdateObject() override;

	};
}
