#pragma once

#include <flounder.h>

using namespace flounder;

class instance : public istandard
{
private:
	ibutton *m_buttonFullscreen;
	ibutton *m_buttonWireframe;
	ibutton *m_buttonExit;

	particlesystem *m_systemTest;
public:
	instance();

	~instance();

	void update() override;
};
