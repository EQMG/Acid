#pragma once

#include <flounder.h>

#include "ais/aiplayer.h"

using namespace flounder;

class instance :
	public istandard
{
private:
	ibutton *m_buttonFullscreen;
	ibutton *m_buttonWireframe;
	ibutton *m_buttonExit;

	float m_time;
	entity *m_terrainDay;
	entity *m_terrainNight;
	entity *m_dome;
	std::vector<aiplayer*> m_ais;
public:
	instance();

	~instance();

	void update() override;
};
