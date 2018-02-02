#pragma once

#include <Uis/UiObject.hpp>
#include <Guis/Gui.hpp>

using namespace Flounder;

namespace Demo
{
	class ContentExit :
		public UiObject
	{
	private:
		Gui *m_background;
	public:
		ContentExit(UiObject *parent);

		~ContentExit();

		void UpdateObject() override;

	};
}
