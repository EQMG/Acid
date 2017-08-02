#include "shadowbox.h"

namespace flounder
{
	vector4 const shadowbox::UP = vector4(0.0f, 1.0f, 0.0f, 0.0f);
	vector4 const shadowbox::FORWARD = vector4(0.0f, 0.0f, -1.0f, 0.0f);

	shadowbox::shadowbox(const matrix4x4 &lightViewMatrix)
	{
		m_lightViewMatrix = new matrix4x4(lightViewMatrix);
		m_aabb = new aabb();

		m_shadowDistance = 0.0f; 
		m_shadowOffset = 0.0f;
		m_farHeight = 0.0f; 
		m_farWidth = 0.0f;
		m_nearHeight = 0.0f; 
		m_nearWidth = 0.0f;
	}

	shadowbox::~shadowbox()
	{
		delete m_lightViewMatrix;
		delete m_aabb;
	}

	void shadowbox::update(const icamera &camera, const float &shadowDistance, const float &shadowOffset)
	{
		m_shadowDistance = shadowDistance;
		m_shadowOffset = shadowOffset;

		updateWidthsAndHeights(camera);

		matrix4x4 *rotation = calculateCameraRotationMatrix(camera);
		vector4 *forwardVector4 = matrix4x4::transform(*rotation, FORWARD, NULL);
		vector3 *forwardVector = new vector3(forwardVector4->m_x, forwardVector4->m_y, forwardVector4->m_z);

		vector3 *toFar = new vector3(*forwardVector);
		toFar->scale(m_shadowDistance);
		vector3 *toNear = new vector3(*forwardVector);
		toNear->scale(camera.getNearPlane());
		vector3 *centreNear = vector3::add(*toNear, *camera.getPosition(), NULL);
		vector3 *centreFar = vector3::add(*toFar, *camera.getPosition(), NULL);

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
				continue;
			}

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

		delete rotation;
		delete forwardVector4;
		delete forwardVector;

		delete toFar;
		delete toNear;
		delete centreNear;
		delete centreFar;

		delete[] points;

		m_aabb->m_maxExtents->m_z += m_shadowOffset;
	}

	void shadowbox::updateWidthsAndHeights(const icamera &camera)
	{
		m_farWidth = m_shadowDistance * tan(__radians(camera.getFOV()));
		m_nearWidth = camera.getNearPlane() * tan(__radians(camera.getFOV()));
		m_farHeight = m_farWidth / display::get()->getAspectRatio();
		m_nearHeight = m_nearWidth / display::get()->getAspectRatio();
	}

	matrix4x4 * shadowbox::calculateCameraRotationMatrix(const icamera &camera)
	{
		matrix4x4 *rotation = new matrix4x4();
		matrix4x4::rotate(*rotation, vector3(0.0f, 1.0f, 0.0f), __radians(camera.getRotation()->m_y), rotation);
		matrix4x4::rotate(*rotation, vector3(1.0f, 0.0f, 0.0f), __radians(-camera.getRotation()->m_x), rotation);
		return rotation;
	}

	vector4 **shadowbox::calculateFrustumVertices(const matrix4x4 &rotation, const vector3 &forwardVector, const vector3 &centreNear, const vector3 &centreFar)
	{
		vector4 *upVector4 = matrix4x4::transform(rotation, UP, NULL);
		vector3 *upVector = new vector3(upVector4->m_x, upVector4->m_y, upVector4->m_z);
		vector3 *rightVector = vector3::cross(forwardVector, *upVector, NULL);
		vector3 *downVector = new vector3(-upVector->m_x, -upVector->m_y, -upVector->m_z);
		vector3 *leftVector = new vector3(-rightVector->m_x, -rightVector->m_y, -rightVector->m_z);
		vector3 *farTop = vector3::add(centreFar, vector3(upVector->m_x * m_farHeight, upVector->m_y * m_farHeight, upVector->m_z * m_farHeight), NULL);
		vector3 *farBottom = vector3::add(centreFar, vector3(downVector->m_x * m_farHeight, downVector->m_y * m_farHeight, downVector->m_z * m_farHeight), NULL);
		vector3 *nearTop = vector3::add(centreNear, vector3(upVector->m_x * m_nearHeight, upVector->m_y * m_nearHeight, upVector->m_z * m_nearHeight), NULL);
		vector3 *nearBottom = vector3::add(centreNear, vector3(downVector->m_x * m_nearHeight, downVector->m_y * m_nearHeight, downVector->m_z * m_nearHeight), NULL);

		vector4 **points = new vector4*[8];
		points[0] = calculateLightSpaceFrustumCorner(*farTop, *rightVector, m_farWidth);
		points[1] = calculateLightSpaceFrustumCorner(*farTop, *leftVector, m_farWidth);
		points[2] = calculateLightSpaceFrustumCorner(*farBottom, *rightVector, m_farWidth);
		points[3] = calculateLightSpaceFrustumCorner(*farBottom, *leftVector, m_farWidth);
		points[4] = calculateLightSpaceFrustumCorner(*nearTop, *rightVector, m_nearWidth);
		points[5] = calculateLightSpaceFrustumCorner(*nearTop, *leftVector, m_nearWidth);
		points[6] = calculateLightSpaceFrustumCorner(*nearBottom, *rightVector, m_nearWidth);
		points[7] = calculateLightSpaceFrustumCorner(*nearBottom, *leftVector, m_nearWidth);

		delete upVector4;
		delete upVector;
		delete rightVector;
		delete downVector;
		delete leftVector;
		delete farTop;
		delete farBottom;
		delete nearTop;
		delete nearBottom;

		return points;
	}

	vector4 * shadowbox::calculateLightSpaceFrustumCorner(const vector3 &startPoint, const vector3 &direction, const float &width)
	{
		vector3 *point = vector3::add(startPoint, vector3(direction.m_x * width, direction.m_y * width, direction.m_z * width), NULL);
		vector4 *point4f = new vector4(point->m_x, point->m_y, point->m_z, 1.0f);
		matrix4x4::transform(*m_lightViewMatrix, *point4f, point4f);
		
		delete point;

		return point4f;
	}

	bool shadowbox::isInBox(const vector3 &position, const float &radius)
	{
		vector4 *entityPos = matrix4x4::transform(*m_lightViewMatrix, vector4(position.m_x, position.m_y, position.m_z, 1.0f), 0);
		float closestX = maths::clamp(entityPos->m_x, m_aabb->m_minExtents->m_x, m_aabb->m_maxExtents->m_x);
		float closestY = maths::clamp(entityPos->m_y, m_aabb->m_minExtents->m_y, m_aabb->m_maxExtents->m_y);
		float closestZ = maths::clamp(entityPos->m_z, m_aabb->m_minExtents->m_z, m_aabb->m_maxExtents->m_z);
		vector3 *closestPoint = new vector3(closestX, closestY, closestZ);
		vector3 *centre = new vector3(entityPos->m_x, entityPos->m_y, entityPos->m_z);
		vector3 *distance = vector3::subtract(*centre, *closestPoint, NULL);
		float disSquared = distance->lengthSquared();

		delete entityPos;
		delete closestPoint;
		delete centre;
		delete distance;

		return disSquared < radius * radius;
	}

	vector3 * shadowbox::getCenter()
	{
		float x = (m_aabb->m_minExtents->m_x + m_aabb->m_maxExtents->m_x) / 2.0f;
		float y = (m_aabb->m_minExtents->m_y + m_aabb->m_maxExtents->m_y) / 2.0f;
		float z = (m_aabb->m_minExtents->m_z + m_aabb->m_maxExtents->m_z) / 2.0f;
		vector4 *centre = new vector4(x, y, z, 1.0f);
		matrix4x4 *invertedLight = new matrix4x4();
		matrix4x4::invert(*m_lightViewMatrix, invertedLight);
		vector4 *result4 = matrix4x4::transform(*invertedLight, *centre, NULL);
		vector3 *result = new vector3(result4->m_x, result4->m_y, result4->m_z);

		delete centre;
		delete invertedLight;
		delete result4;

		return result;
	}

	float shadowbox::getWidth()
	{
		return m_aabb->m_maxExtents->m_x - m_aabb->m_minExtents->m_x;
	}

	float shadowbox::getHeight()
	{
		return m_aabb->m_maxExtents->m_y - m_aabb->m_minExtents->m_y;
	}

	float shadowbox::getLength()
	{
		return m_aabb->m_maxExtents->m_z - m_aabb->m_minExtents->m_z;
	}
}
