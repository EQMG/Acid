#include "ray.h"

namespace flounder {
	ray::ray(const bool useMouse, vector2 *screenStart)
	{
		m_useMouse = useMouse;
		m_screenStart = screenStart;

		m_viewMatrix = new matrix4x4();
		m_projectionMatrix = new matrix4x4();

		m_origin = new vector3();
		m_currentRay = new vector3();

		m_normalizedCoords = new vector2();
		m_clipCoords = new vector4();
		m_eyeCoords = new vector4();

		m_invertedProjection = new matrix4x4();
		m_invertedView = new matrix4x4();
		m_rayWorld = new vector4();
	}

	ray::~ray()
	{
		delete m_screenStart;

		delete m_origin;
		delete m_currentRay;

		delete m_viewMatrix;
		delete m_projectionMatrix;

		delete m_normalizedCoords;
		delete m_clipCoords;
		delete m_eyeCoords;

		delete m_invertedProjection;
		delete m_invertedView;
		delete m_rayWorld;
	}

	void ray::update(vector3 *currentPosition, vector2 *mousePosition, matrix4x4 *viewMatrix, matrix4x4 *projectionMatrix)
	{
		m_origin->set(currentPosition);

		if (m_useMouse)
		{
			updateNormalisedDeviceCoordinates(mousePosition->x, mousePosition->y);
		}
		else
		{
			if (m_screenStart != 0)
			{
				m_normalizedCoords->set(m_screenStart);
			}
			else
			{
				m_normalizedCoords->set(0.0f, 0.0f);
			}
		}

		m_viewMatrix->set(viewMatrix);
		m_projectionMatrix->set(projectionMatrix);
		m_clipCoords->set(m_normalizedCoords->x, m_normalizedCoords->y, -1.0f, 1.0f);
		updateEyeCoords(m_clipCoords);
		updateWorldCoords(m_eyeCoords);
	}

	vector3 *ray::getPointOnRay(const float distance, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		return vector3::add(m_origin, destination->set(m_currentRay)->scale(distance), destination);
	}

	vector3 *ray::convertToScreenSpace(vector3 *position, vector3 *destination)
	{
		if (destination == NULL)
		{
			destination = new vector3();
		}

		vector4 *coords = new vector4(position->x, position->y, position->z, 1.0f);
		matrix4x4::transform(m_viewMatrix, coords, coords);
		matrix4x4::transform(m_projectionMatrix, coords, coords);

		if (coords->w < 0.0f)
		{
			return NULL;
		}

		return destination->set((coords->x / coords->w + 1.0f) / 2.0f, 1.0f - (coords->y / coords->w + 1.0f) / 2.0f, coords->z);
	}

	void ray::updateNormalisedDeviceCoordinates(const float mouseX, const float mouseY)
	{
		float x = (2.0f * mouseX) - 1.0f;
		float y = (2.0f * mouseY) - 1.0f;
		m_normalizedCoords->set(x, -y);
	}

	void ray::updateEyeCoords(vector4 *clipCoords)
	{
		m_invertedProjection = matrix4x4::invert(m_projectionMatrix, m_invertedProjection);
		matrix4x4::transform(m_invertedProjection, m_clipCoords, m_eyeCoords);
		m_eyeCoords->set(m_eyeCoords->x, m_eyeCoords->y, -1.0f, 0.0f);
	}

	void ray::updateWorldCoords(vector4 *eyeCoords)
	{
		matrix4x4::invert(m_viewMatrix, m_invertedView);
		matrix4x4::transform(m_invertedView, m_eyeCoords, m_rayWorld);
		m_currentRay->set(m_rayWorld->x, m_rayWorld->y, m_rayWorld->z);
	}
}
