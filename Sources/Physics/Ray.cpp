#include "Ray.hpp"

namespace acid
{
const Ray Ray::Zero = Ray(false, Vector2::Zero);

Ray::Ray(const bool& useMouse, const Vector2& screenStart) : m_useMouse(useMouse), m_screenStart(screenStart) {}

void Ray::Update(const Vector3& currentPosition, const Vector2& mousePosition, const Matrix4& viewMatrix, const Matrix4& projectionMatrix)
{
	m_origin = currentPosition;

	if(m_useMouse)
		{
			UpdateNormalisedDeviceCoordinates(mousePosition.m_x, mousePosition.m_y);
		}
	else
		{
			m_normalizedCoords = m_screenStart;
		}

	m_viewMatrix = viewMatrix;
	m_projectionMatrix = projectionMatrix;
	m_clipCoords = Vector4(m_normalizedCoords.m_x, m_normalizedCoords.m_y, -1.0f, 1.0f);
	UpdateEyeCoords();
	UpdateWorldCoords();
}

Vector3 Ray::GetPointOnRay(const float& distance) const
{
	Vector3 vector = Vector3(distance * m_currentRay);
	return m_origin + vector;
}

Vector3 Ray::ConvertToScreenSpace(const Vector3& position) const
{
	Vector4 coords = Vector4(position);
	coords = m_viewMatrix.Transform(coords);
	coords = m_projectionMatrix.Transform(coords);

	if(coords.m_w < 0.0f)
		{
			return Vector3();
		}

	return Vector3((coords.m_x / coords.m_w + 1.0f) / 2.0f, 1.0f - (coords.m_y / coords.m_w + 1.0f) / 2.0f, coords.m_z);
}

void Ray::UpdateNormalisedDeviceCoordinates(const float& mouseX, const float& mouseY)
{
	m_normalizedCoords.m_x = (2.0f * mouseX) - 1.0f;
	m_normalizedCoords.m_y = (2.0f * mouseY) - 1.0f;
}

void Ray::UpdateEyeCoords()
{
	m_invertedProjection = m_projectionMatrix.Inverse();
	m_eyeCoords = m_invertedProjection.Transform(m_clipCoords);
	m_eyeCoords = Vector4(m_eyeCoords.m_x, m_eyeCoords.m_y, -1.0f, 0.0f);
}

void Ray::UpdateWorldCoords()
{
	m_invertedView = m_viewMatrix.Inverse();
	m_rayWorld = m_invertedView.Transform(m_eyeCoords);
	m_currentRay = m_rayWorld;
}
}
