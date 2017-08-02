#include "shadows.h"

namespace flounder
{
	shadows::shadows() :
		imodule()
	{
		m_lightPosition = new vector3(0.5f, 0.0f, 0.5f);
		m_brightnessBoost = 0.1f;

		m_shadowSize = 8192;
		m_shadowPCF = 0;
		m_shadowBias = 0.001f;
		m_shadowDarkness = 0.6f;
		m_shadowTransition = 11.0f;

		m_shadowFactor = 1.0f;

		m_shadowBoxOffset = 25.0f;
		m_shadowBoxDistance = 40.0f;

		m_projectionMatrix = new matrix4x4();
		m_lightViewMatrix = new matrix4x4();
		m_projectionViewMatrix = new matrix4x4();
		m_shadowMapSpaceMatrix = new matrix4x4();
		m_offset = createOffset();

		m_shadowBox = new shadowbox(*m_lightViewMatrix);
		m_shadowAabb = new aabb();
	}

	shadows::~shadows()
	{
		delete m_lightPosition;

		delete m_projectionMatrix;
		delete m_lightViewMatrix;
		delete m_projectionViewMatrix;
		delete m_shadowMapSpaceMatrix;
		delete m_offset;

		delete m_shadowBox;
		delete m_shadowAabb;
	}

	void shadows::update()
	{
		m_shadowBox->update(*camera::get()->getCamera(), m_shadowBoxDistance, m_shadowBoxOffset);
		updateOrthographicProjectionMatrix(m_shadowBox->getWidth(), m_shadowBox->getHeight(), m_shadowBox->getLength());
		updateLightViewMatrix(m_lightPosition, m_shadowBox->getCenter());
		matrix4x4::multiply(*m_projectionMatrix, *m_lightViewMatrix, m_projectionViewMatrix);
		matrix4x4::multiply(*m_offset, *m_projectionViewMatrix, m_shadowMapSpaceMatrix);

		m_shadowAabb->m_minExtents->set(*m_shadowBox->getAabb()->m_minExtents);
		m_shadowAabb->m_maxExtents->set(*m_shadowBox->getAabb()->m_maxExtents);
		m_shadowAabb->update(*camera::get()->getCamera()->getPosition(), vector3(0.0f, 0.0f, 0.0f), 2.5f, m_shadowAabb);
	}

	void shadows::updateOrthographicProjectionMatrix(const float &width, const float &height, const float &length)
	{
		m_projectionMatrix->setIdentity();
		m_projectionMatrix->m_00 = 2.0f / width;
		m_projectionMatrix->m_11 = 2.0f / height;
		m_projectionMatrix->m_22 = -2.0f / length;
		m_projectionMatrix->m_33 = 1.0f;
	}

	void shadows::updateLightViewMatrix(vector3 *direction, vector3 *position)
	{
		direction->normalize();
		position->negate();

		m_lightViewMatrix->setIdentity();
		float pitch = acos(vector2(direction->m_x, direction->m_z).length());
		matrix4x4::rotate(*m_lightViewMatrix, vector3(1.0f, 0.0f, 0.0f), pitch, m_lightViewMatrix);
		float yaw = __degrees(atan(direction->m_x / direction->m_z));
		yaw = direction->m_z > 0.0f ? yaw - 180.0f : yaw;
		matrix4x4::rotate(*m_lightViewMatrix, vector3(0.0f, 1.0f, 0.0f), -__radians(yaw), m_lightViewMatrix);
		matrix4x4::translate(*m_lightViewMatrix, *position, m_lightViewMatrix);
	}

	matrix4x4 *shadows::createOffset()
	{
		matrix4x4 *offset = new matrix4x4();
		matrix4x4::translate(*offset, vector3(0.5f, 0.5f, 0.5f), offset);
		matrix4x4::scale(*offset, vector3(0.5f, 0.5f, 0.5f), offset);
		return offset;
	}
}
