#pragma once

#include <Flounder.hpp>

using namespace Flounder;

namespace Demo
{
	class OverlayPaused :
		public UiObject
	{
	private:
		Text *m_textExample;
		Gui *m_guiExample;
	public:
		OverlayPaused(UiObject *parent);

		~OverlayPaused();

		virtual void UpdateObject() override;
	};
}
