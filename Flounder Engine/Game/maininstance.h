#pragma once

#include "../Sources/standards/istandard.h"

#include "../Sources/inputs/buttonkeyboard.h"
#include "../Sources/inputs/buttonmouse.h"

namespace flounder {
	class maininstance : public istandard
	{
	private:
		ibutton *m_buttonFullscreen;
		ibutton *m_buttonExit;
	public:
		maininstance();

		~maininstance();

		void update() override;
	};
}
