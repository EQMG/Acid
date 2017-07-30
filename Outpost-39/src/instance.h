#pragma once

#include <flounder.h>

#include "ais/aiplayer.h"
#include "ais/aitasktargetx.h"
#include "ais/aitasklevel.h"
#include "ais/aitasksleep.h"

using namespace flounder;

class instance :
	public istandard
{
private:
	ibutton *m_buttonFullscreen;
	ibutton *m_buttonWireframe;
	ibutton *m_buttonExit;

	entity *m_terrainDay;
	entity *m_terrainNight;
	entity *m_dome;
	std::vector<aiplayer*> m_ais;

	timer *m_timerFortune;
public:
	static const float DAY_LENGTH;

	instance();

	~instance();

	void update() override;
};
