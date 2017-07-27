#pragma once

#include "../events/events.h"
#include "../events/eventtime.h"
#include "../fonts/fonts.h"
#include "../fonts/text.h"
#include "../visual/driverslide.h"
#include "guis.h"
#include "guialign.h"
#include "gui.h"
#include "screenobject.h"

namespace flounder
{
	class overlaystartup :
		public screenobject
	{
	private:
		gui *m_background;
		gui *m_logo;
		text *m_text;
		bool m_starting;

	public:
		overlaystartup(screenobject *parent);

		~overlaystartup();

		void updateObject() override;

		inline bool getStarting() { return m_starting; }

		inline void setStarting(const bool &starting) { m_starting = starting; }
	};
}
