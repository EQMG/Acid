#pragma once

#include <flounder.hpp>

#include "entities/entitytest.hpp"

using namespace flounder;

class instance :
	public istandard
{
private:
	ibutton *m_buttonFullscreen;
	ibutton *m_buttonLockMouse;
	ibutton *m_buttonExit;

	particlesystem *m_systemTest;
public:
	instance();

	~instance();

	void update() override;
};
