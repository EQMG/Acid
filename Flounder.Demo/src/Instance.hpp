#pragma once

#include <Flounder.hpp>

#include "entities/EntityTest.hpp"

using namespace Flounder;

class Instance :
	public IStandard
{
private:
	IButton *m_buttonFullscreen;
	IButton *m_buttonLockMouse;
	IButton *m_buttonExit;

	ParticleSystem *m_systemTest;
public:
	Instance();

	~Instance();

	void Update() override;
};
