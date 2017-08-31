#include "ray.hpp"

namespace Flounder
{
	ray::ray(const bool &useMouse, const Vector2 &screenStart) :
		m_useMouse(useMouse),
		m_screenStart(new Vector2(screenStart)),
		m_viewMatrix(new Matrix4()),
		m_projectionMatrix(new Matrix4()),
		m_normalizedCoords(new Vector2()),
		m_clipCoords(new Vector4()),
		m_eyeCoords(new Vector4()),
		m_invertedProjection(new Matrix4()),
		m_invertedView(new Matrix4()),
		m_rayWorld(new Vector4()),
		m_origin(new Vector3()),
		m_currentRay(new Vector3())
	{
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

	void ray::update(const Vector3 &currentPosition, const Vector2 &mousePosition, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix)
	{
		m_origin->set(currentPosition);

		if (m_useMouse)
		{
			updateNormalisedDeviceCoordinates(mousePosition.m_x, mousePosition.m_y);
		}
		else
		{
			if (m_screenStart != nullptr)
			{
				m_normalizedCoords->set(*m_screenStart);
			}
			else
			{
				m_normalizedCoords->set(0.0f, 0.0f);
			}
		}

		m_viewMatrix->set(viewMatrix);
		m_projectionMatrix->set(projectionMatrix);
		m_clipCoords->set(m_normalizedCoords->m_x, m_normalizedCoords->m_y, -1.0f, 1.0f);
		updateEyeCoords(m_clipCoords);
		updateWorldCoords(m_eyeCoords);
	}

	Vector3 *ray::getPointOnRay(const float &distance, Vector3 *destination) const
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return Vector3::add(*m_origin, *destination->set(*m_currentRay)->scale(distance), destination);
	}

	Vector3 *ray::convertToScreenSpace(const Vector3 &position, Vector3 *destination) const
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		Vector4 *coords = new Vector4(position);
		Matrix4::transform(*m_viewMatrix, *coords, coords);
		Matrix4::transform(*m_projectionMatrix, *coords, coords);

		if (coords->m_w < 0.0f)
		{
			return nullptr;
		}

		return destination->set((coords->m_x / coords->m_w + 1.0f) / 2.0f, 1.0f - (coords->m_y / coords->m_w + 1.0f) / 2.0f, coords->m_z);
	}

	void ray::updateNormalisedDeviceCoordinates(const float &mouseX, const float &mouseY)
	{
		float x = (2.0f * mouseX) - 1.0f;
		float y = (2.0f * mouseY) - 1.0f;
		m_normalizedCoords->set(x, -y);
	}

	void ray::updateEyeCoords(Vector4 *clipCoords)
	{
		m_invertedProjection = Matrix4::invert(*m_projectionMatrix, m_invertedProjection);
		Matrix4::transform(*m_invertedProjection, *m_clipCoords, m_eyeCoords);
		m_eyeCoords->set(m_eyeCoords->m_x, m_eyeCoords->m_y, -1.0f, 0.0f);
	}

	void ray::updateWorldCoords(Vector4 *eyeCoords)
	{
		Matrix4::invert(*m_viewMatrix, m_invertedView);
		Matrix4::transform(*m_invertedView, *m_eyeCoords, m_rayWorld);
		m_currentRay->set(*m_rayWorld);
	}
}
