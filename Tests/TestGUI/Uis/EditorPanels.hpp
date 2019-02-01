#pragma once

#include <Guis/Gui.hpp>
#include "Hierarchy.hpp"
#include "Inspector.hpp"
#include "Console.hpp"

using namespace acid;

namespace test
{
	class EditorPanels :
		public UiObject
	{
	public:
		explicit EditorPanels(UiObject *parent);

		void UpdateObject() override;
	private:
		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Hierarchy> m_hierarchy;
		std::unique_ptr<Inspector> m_inspector;
		std::unique_ptr<Console> m_console;

		std::unique_ptr<Gui> m_banner;
	};
}
