#include "mainplayer.h"

mainplayer::mainplayer() :
	iplayer()
{
	m_position = new vector3();
	m_rotation = new vector3();
}

mainplayer::~mainplayer()
{
	delete m_position;
	delete m_rotation;
}

void mainplayer::update()
{
	m_rotation->y += (float)framework::get()->getDelta() * 5.0f;
	m_rotation->y = maths::normalizeAngle(m_rotation->y);
}
