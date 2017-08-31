#include "shadowbox.hpp"

namespace Flounder
{
	shadowbox::shadowbox() :
		m_lightDirection(new vector3()),
		m_shadowOffset(0.0f),
		m_shadowDistance(0.0f),
		m_projectionMatrix(new matrix4x4()),
		m_lightViewMatrix(new matrix4x4()),
		m_projectionViewMatrix(new matrix4x4()),
		m_shadowMapSpaceMatrix(new matrix4x4()),
		m_offset(createOffset()),
		m_centre(new vector3()),
		m_farHeight(0.0f),
		m_farWidth(0.0f),
		m_nearHeight(0.0f),
		m_nearWidth(0.0f),
		m_aabb(new aabb())
	{
	}

	shadowbox::~shadowbox()
	{
		delete m_lightDirection;

		delete m_projectionMatrix;
		delete m_lightViewMatrix;
		delete m_projectionViewMatrix;
		delete m_shadowMapSpaceMatrix;
		delete m_offset;
		delete m_centre;

		delete m_aabb;
	}

	void shadowbox::update(const ICamera &camera, const vector3 &lightPosition, const float &shadowOffset, const float &shadowDistance)
	{
		m_lightDirection->set(lightPosition);
		m_lightDirection->normalize();
		m_shadowOffset = shadowOffset;
		m_shadowDistance = shadowDistance;

		updateShadowBox(camera);
		updateOrthoProjectionMatrix();
		updateCenter();
		updateLightViewMatrix();
		updateViewShadowMatrix();
	}

	matrix4x4 *shadowbox::createOffset()
	{
		matrix4x4 *offset = new matrix4x4();
		matrix4x4::translate(*offset, vector3(0.5f, 0.5f, 0.5f), offset);
		matrix4x4::scale(*offset, vector3(0.5f, 0.5f, 0.5f), offset);
		return offset;
	}

	void shadowbox::updateShadowBox(const ICamera &camera)
	{
		updateWidthsAndHeights(camera);

		matrix4x4 *rotation = matrix4x4::viewMatrix(vector3(), *camera.GetPosition(), nullptr);
		vector4 *forwardVector4 = matrix4x4::transform(*rotation, vector4(0.0f, 0.0f, -1.0f, 0.0f), nullptr);
		vector3 *forwardVector = new vector3(*forwardVector4);

		vector3 *toFar = new vector3(*forwardVector);
		toFar->scale(m_shadowDistance);
		vector3 *toNear = new vector3(*forwardVector);
		toNear->scale(camera.GetNearPlane());
		vector3 *centreNear = vector3::add(*toNear, *camera.GetPosition(), nullptr);
		vector3 *centreFar = vector3::add(*toFar, *camera.GetPosition(), nullptr);

		vector4 **points = calculateFrustumVertices(*rotation, *forwardVector, *centreNear, *centreFar);

		for (int i = 0; i < 8; i++)
		{
			vector4 *point = points[i];

			if (i == 0)
			{
				m_aabb->m_minExtents->m_x = point->m_x;
				m_aabb->m_maxExtents->m_x = point->m_x;
				m_aabb->m_minExtents->m_y = point->m_y;
				m_aabb->m_maxExtents->m_y = point->m_y;
				m_aabb->m_minExtents->m_z = point->m_z;
				m_aabb->m_maxExtents->m_z = point->m_z;
			}
			else
			{
				if (point->m_x > m_aabb->m_maxExtents->m_x)
				{
					m_aabb->m_maxExtents->m_x = point->m_x;
				}
				else if (point->m_x < m_aabb->m_minExtents->m_x)
				{
					m_aabb->m_minExtents->m_x = point->m_x;
				}

				if (point->m_y > m_aabb->m_maxExtents->m_y)
				{
					m_aabb->m_maxExtents->m_y = point->m_y;
				}
				else if (point->m_y < m_aabb->m_minExtents->m_y)
				{
					m_aabb->m_minExtents->m_y = point->m_y;
				}

				if (point->m_z > m_aabb->m_maxExtents->m_z)
				{
					m_aabb->m_maxExtents->m_z = point->m_z;
				}
				else if (point->m_z < m_aabb->m_minExtents->m_z)
				{
					m_aabb->m_minExtents->m_z = point->m_z;
				}
			}

			delete point;
		}

		m_aabb->m_maxExtents->m_z += m_shadowOffset;

		delete[] points;
		delete centreFar;
		delete centreNear;

		delete toFar;
		delete toNear;

		delete rotation;
		delete forwardVector;
		delete forwardVector4;
	}

	void shadowbox::updateWidthsAndHeights(const ICamera &camera)
	{
		m_farWidth = m_shadowDistance * tan(__radians(camera.GetFov()));
		m_nearWidth = camera.GetNearPlane() * tan(__radians(camera.GetFov()));
		m_farHeight = m_farWidth / static_cast<float>(display::get()->getAspectRatio());
		m_nearHeight = m_nearWidth / static_cast<float>(display::get()->getAspectRatio());
	}

	vector4 **shadowbox::calculateFrustumVertices(const matrix4x4 &rotation, const vector3 &forwardVector, const vector3 &centreNear, const vector3 &centreFar)
	{
		vector4 *upVector4 = matrix4x4::transform(rotation, vector4(0.0f, 1.0f, 0.0f, 0.0f), nullptr);
		vector3 *upVector = new vector3(*upVector4);
		vector3 *rightVector = vector3::cross(forwardVector, *upVector, nullptr);
		vector3 *downVector = new vector3(*upVector);
		downVector->negate();
		vector3 *leftVector = new vector3(*rightVector);
		leftVector->negate();

		vector3 *farUpVector = new vector3(*upVector);
		farUpVector->scale(m_farHeight);
		vector3 *farDownVector = new vector3(*downVector);
		farDownVector->scale(m_farHeight);
		vector3 *nearUpVector = new vector3(*upVector);
		nearUpVector->scale(m_nearHeight);
		vector3 *nearDownVector = new vector3(*downVector);
		nearDownVector->scale(m_nearHeight);

		vector3 *farTop = vector3::add(centreFar, *farUpVector, nullptr);
		vector3 *farBottom = vector3::add(centreFar, *farDownVector, nullptr);
		vector3 *nearTop = vector3::add(centreNear, *nearUpVector, nullptr);
		vector3 *nearBottom = vector3::add(centreNear, *nearDownVector, nullptr);

		vector4 **points = new vector4*[8];
		points[0] = calculateLightSpaceFrustumCorner(*farTop, *rightVector, m_farWidth);
		points[1] = calculateLightSpaceFrustumCorner(*farTop, *leftVector, m_farWidth);
		points[2] = calculateLightSpaceFrustumCorner(*farBottom, *rightVector, m_farWidth);
		points[3] = calculateLightSpaceFrustumCorner(*farBottom, *leftVector, m_farWidth);
		points[4] = calculateLightSpaceFrustumCorner(*nearTop, *rightVector, m_nearWidth);
		points[5] = calculateLightSpaceFrustumCorner(*nearTop, *leftVector, m_nearWidth);
		points[6] = calculateLightSpaceFrustumCorner(*nearBottom, *rightVector, m_nearWidth);
		points[7] = calculateLightSpaceFrustumCorner(*nearBottom, *leftVector, m_nearWidth);

		delete nearBottom;
		delete nearTop;
		delete farBottom;
		delete farTop;

		delete nearDownVector;
		delete nearUpVector;
		delete farDownVector;
		delete farUpVector;

		delete leftVector;
		delete downVector;
		delete rightVector;
		delete upVector;
		delete upVector4;

		return points;
	}

	vector4 *shadowbox::calculateLightSpaceFrustumCorner(const vector3 &startPoint, const vector3 &direction, const float &width)
	{
		vector3 *point = vector3::add(startPoint, *vector3(direction).scale(width), nullptr);
		vector4 *point4 = new vector4(*point);
		matrix4x4::transform(*m_lightViewMatrix, *point4, point4);

		delete point;

		return point4;
	}

	void shadowbox::updateCenter()
	{
		float x = (m_aabb->m_minExtents->m_x + m_aabb->m_maxExtents->m_x) / 2.0f;
		float y = (m_aabb->m_minExtents->m_y + m_aabb->m_maxExtents->m_y) / 2.0f;
		float z = (m_aabb->m_minExtents->m_z + m_aabb->m_maxExtents->m_z) / 2.0f;
		matrix4x4 *invertedLight = matrix4x4::invert(*m_lightViewMatrix, nullptr);
		vector4 *centre4 = matrix4x4::transform(*invertedLight, vector4(x, y, z, 1.0f), nullptr);

		m_centre->set(*centre4);

		delete centre4;
		delete invertedLight;
	}

	void shadowbox::updateOrthoProjectionMatrix()
	{
		m_projectionMatrix->setIdentity();
		m_projectionMatrix->m_00 = 2.0f / m_aabb->getWidth();
		m_projectionMatrix->m_11 = 2.0f / m_aabb->getHeight();
		m_projectionMatrix->m_22 = -2.0f / m_aabb->getDepth();
		m_projectionMatrix->m_33 = 1.0f;
	}

	void shadowbox::updateLightViewMatrix()
	{
		m_centre->negate();

		m_lightViewMatrix->setIdentity();
		float pitch = acos(vector2(m_lightDirection->m_x, m_lightDirection->m_z).length());
		matrix4x4::rotate(*m_lightViewMatrix, vector3(1.0f, 0.0f, 0.0f), pitch, m_lightViewMatrix);
		float yaw = __degrees(atan(m_lightDirection->m_x / m_lightDirection->m_z));

		if (m_lightDirection->m_z > 0.0f)
		{
			yaw -= 180.0f;
		}

		matrix4x4::rotate(*m_lightViewMatrix, vector3(0.0f, 1.0f, 0.0f), -__radians(yaw), m_lightViewMatrix);
		matrix4x4::translate(*m_lightViewMatrix, *m_centre, m_lightViewMatrix);

		m_centre->negate();
	}

	void shadowbox::updateViewShadowMatrix()
	{
		matrix4x4::multiply(*m_projectionMatrix, *m_lightViewMatrix, m_projectionViewMatrix);
		matrix4x4::multiply(*m_offset, *m_projectionViewMatrix, m_shadowMapSpaceMatrix);
	}

	bool shadowbox::isInBox(const vector3 &position, const float &radius)
	{
		vector4 *entityPos = matrix4x4::transform(*m_lightViewMatrix, vector4(position), nullptr);

		vector3 *closestPoint = new vector3();
		closestPoint->m_x = maths::clamp(entityPos->m_x, m_aabb->m_minExtents->m_x, m_aabb->m_maxExtents->m_x);
		closestPoint->m_y = maths::clamp(entityPos->m_y, m_aabb->m_minExtents->m_y, m_aabb->m_maxExtents->m_y);
		closestPoint->m_z = maths::clamp(entityPos->m_z, m_aabb->m_minExtents->m_z, m_aabb->m_maxExtents->m_z);

		vector3 *centre = new vector3(*entityPos);
		vector3 *distance = vector3::subtract(*centre, *closestPoint, nullptr);
		float disSquared = distance->lengthSquared();

		delete distance;
		delete centre;
		delete closestPoint;
		delete entityPos;

		return disSquared < radius * radius;
	}
}
