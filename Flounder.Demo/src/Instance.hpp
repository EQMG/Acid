#pragma once

#include <Flounder.hpp>

#include "entities/EntityTest.hpp"

using namespace Flounder;

class Instance :
	public IStandard
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
