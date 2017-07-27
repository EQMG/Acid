#include "maincamera.h"

maincamera::maincamera()
{
	m_position = new vector3();
	m_rotation = new vector3();

	m_viewMatrix = new matrix4x4();
	m_projectionMatrix = new matrix4x4();

	m_viewFrustum = new frustum();
	m_viewRay = new ray(false, new vector2(0.5f, 0.5f));
}

maincamera::~maincamera()
{
	delete m_position;
	delete m_rotation;

	delete m_viewMatrix;
	delete m_projectionMatrix;

	delete m_viewFrustum;
	delete m_viewRay;
}

void maincamera::update(const iplayer &player)
{
	if (&player != NULL)
	{
		m_position->set(*player.getPosition());
		m_rotation->set(*player.getRotation());
	}

	matrix4x4::viewMatrix(*m_position, *m_rotation, m_viewMatrix);
	matrix4x4::perspectiveMatrix(getFOV(), static_cast<float>(display::get()->getAspectRatio()), getNearPlane(), getFarPlane(), m_projectionMatrix);

	m_viewFrustum->update(*m_projectionMatrix, *m_viewMatrix);
	m_viewRay->update(*m_position, vector2(static_cast<float>(mouse::get()->getPositionX()), static_cast<float>(mouse::get()->getPositionY())), *m_viewMatrix, *m_projectionMatrix);
}

void maincamera::reflect(const float &waterHeight)
{
	throw std::logic_error("Function maincamera::reflect not yet implemented!");
}
