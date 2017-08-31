#pragma once

#include <flounder.hpp>

#include "entities/entitytest.hpp"

using namespace Flounder;

class Instance :
	public istandard
{
private:
	ibutton *m_buttonFullscreen;
	ibutton *m_buttonLockMouse;
	ibutton *m_buttonExit;

	particlesystem *m_systemTest;
public:
	Instance();

	~Instance();

	void Update() override;
};
