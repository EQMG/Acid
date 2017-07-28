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
	//if (!uis::get()->getManager()->isGamePaused())
	//{
	//m_rotation->m_y += static_cast<float>(framework::get()->getDelta()) * 16.20f;
	//m_rotation->m_y = maths::normalizeAngle(m_rotation->m_y);
	//}
}
