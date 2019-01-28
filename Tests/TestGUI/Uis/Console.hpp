#pragma once

#include <Uis/UiSection.hpp>
#include <Uis/UiPanel.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test
{
	class Console :
		public UiPanel
	{
	private:
		std::unique_ptr<Text> m_text;
		Timer m_timerChange;
	public:
		explicit Console(UiObject *parent);

		void UpdateObject() override;
	};
}
