#include "Ray.hpp"

namespace Flounder
{
	Ray::Ray(const bool &useMouse, const Vector2 &screenStart) :
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

	Ray::~Ray()
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

	void Ray::Update(const Vector3 &currentPosition, const Vector2 &mousePosition, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix)
	{
		m_origin->Set(currentPosition);

		if (m_useMouse)
		{
			UpdateNormalisedDeviceCoordinates(mousePosition.m_x, mousePosition.m_y);
		}
		else
		{
			if (m_screenStart != nullptr)
			{
				m_normalizedCoords->Set(*m_screenStart);
			}
			else
			{
				m_normalizedCoords->Set(0.0f, 0.0f);
			}
		}

		m_viewMatrix->Set(viewMatrix);
		m_projectionMatrix->Set(projectionMatrix);
		m_clipCoords->Set(m_normalizedCoords->m_x, m_normalizedCoords->m_y, -1.0f, 1.0f);
		UpdateEyeCoords(m_clipCoords);
		UpdateWorldCoords(m_eyeCoords);
	}

	Vector3 *Ray::GetPointOnRay(const float &distance, Vector3 *destination) const
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		return Vector3::Add(*m_origin, *destination->Set(*m_currentRay)->Scale(distance), destination);
	}

	Vector3 *Ray::ConvertToScreenSpace(const Vector3 &position, Vector3 *destination) const
	{
		if (destination == nullptr)
		{
			destination = new Vector3();
		}

		Vector4 *coords = new Vector4(position);
		Matrix4::Transform(*m_viewMatrix, *coords, coords);
		Matrix4::Transform(*m_projectionMatrix, *coords, coords);

		if (coords->m_w < 0.0f)
		{
			return nullptr;
		}

		return destination->Set(
			(coords->m_x / coords->m_w + 1.0f) / 2.0f,
			1.0f - (coords->m_y / coords->m_w + 1.0f) / 2.0f, coords->m_z
		);
	}

	void Ray::UpdateNormalisedDeviceCoordinates(const float &mouseX, const float &mouseY)
	{
		float x = (2.0f * mouseX) - 1.0f;
		float y = (2.0f * mouseY) - 1.0f;
		m_normalizedCoords->Set(x, -y);
	}

	void Ray::UpdateEyeCoords(Vector4 *clipCoords)
	{
		m_invertedProjection = Matrix4::Invert(*m_projectionMatrix, m_invertedProjection);
		Matrix4::Transform(*m_invertedProjection, *m_clipCoords, m_eyeCoords);
		m_eyeCoords->Set(m_eyeCoords->m_x, m_eyeCoords->m_y, -1.0f, 0.0f);
	}

	void Ray::UpdateWorldCoords(Vector4 *eyeCoords)
	{
		Matrix4::Invert(*m_viewMatrix, m_invertedView);
		Matrix4::Transform(*m_invertedView, *m_eyeCoords, m_rayWorld);
		m_currentRay->Set(*m_rayWorld);
	}
}
