#include "maincamera.h"

namespace flounder {
	float maincamera::getNearPlane()
	{
		return 0.1f;
	}

	float maincamera::getFarPlane()
	{
		return 100.0f;
	}

	float maincamera::getFOV()
	{
		return 60.0f;
	}

	void maincamera::update(iplayer *player)
	{
		if (player != NULL)
		{
			m_position->set(player->getPosition());
			m_rotation->set(player->getRotation());
		}

		matrix4x4::viewMatrix(m_position, m_rotation, m_viewMatrix);
		matrix4x4::perspectiveMatrix(getFOV(), (float) display::get()->getAspectRatio(), getNearPlane(), getFarPlane(), m_projectionMatrix);

		m_viewFrustum->update(m_projectionMatrix, m_viewMatrix);
		m_viewRay->update(m_position, &vector2((float) mouse::get()->getPositionX(), (float) mouse::get()->getPositionY()), m_viewMatrix, m_projectionMatrix);
	}

	void maincamera::reflect(float waterHeight)
	{
		throw std::logic_error("Function maincamera::reflect not yet implemented!");
	}
}