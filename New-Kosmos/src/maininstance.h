#pragma once

#include <flounder.h>

using namespace flounder;

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
