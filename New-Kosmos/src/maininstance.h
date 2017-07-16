#pragma once

#include <standards/istandard.h>
#include <renderer/renderer.h>

#include <inputs/buttonkeyboard.h>
#include <inputs/buttonmouse.h>

namespace flounder {
	class maininstance : public istandard
	{
	private:
		ibutton *m_buttonFullscreen;
		ibutton *m_buttonWireframe;
		ibutton *m_buttonExit;
	public:
		maininstance();

		~maininstance();

		void update() override;
	};
}
