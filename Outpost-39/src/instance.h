#pragma once

#include <flounder.h>

using namespace flounder;

class instance : public istandard
{
private:
	ibutton *m_buttonFullscreen;
	ibutton *m_buttonWireframe;
	ibutton *m_buttonExit;
public:
	instance();

	~instance();

	void update() override;
};
