#include "ShadowBox.hpp"

#include "Devices/Window.hpp"
#include "Graphics/Graphics.hpp"
#include "Maths/Maths.hpp"

namespace acid
{
ShadowBox::ShadowBox()
{
	// Creates the offset for part of the conversion to shadow map space.
	m_offset = m_offset.Translate(Vector3f(0.5f));
	m_offset = m_offset.Scale(Vector3f(0.5f));
}

void ShadowBox::Update(const Camera &camera, const Vector3f &lightDirection, const float &shadowOffset, const float &shadowDistance)
{
	m_lightDirection = lightDirection;
	m_shadowOffset = shadowOffset;
	m_shadowDistance = shadowDistance;

	UpdateShadowBox(camera);
	UpdateOrthoProjectionMatrix();
	UpdateCenter();
	UpdateLightViewMatrix();
	UpdateViewShadowMatrix();
}

bool ShadowBox::IsInBox(const Vector3f &position, const float &radius) const
{
	auto entityPos{m_lightViewMatrix.Transform(Vector4f(position))};

	Vector3f closestPoint;
	closestPoint.m_x = std::clamp(entityPos.m_x, m_minExtents.m_x, m_maxExtents.m_x);
	closestPoint.m_y = std::clamp(entityPos.m_y, m_minExtents.m_y, m_maxExtents.m_y);
	closestPoint.m_z = std::clamp(entityPos.m_z, m_minExtents.m_z, m_maxExtents.m_z);

	Vector3f centre{entityPos};
	auto distance{centre - closestPoint};
	auto distanceSquared{distance.LengthSquared()};

	return distanceSquared < radius * radius;
}

void ShadowBox::UpdateShadowBox(const Camera &camera)
{
	UpdateSizes(camera);

	Matrix4 rotation;
	rotation = rotation.Rotate(-camera.GetRotation().m_y, Vector3f::Up);
	rotation = rotation.Rotate(-camera.GetRotation().m_x, Vector3f::Right);

	Vector3f forwardVector{rotation.Transform(Vector4f(0.0f, 0.0f, -1.0f, 0.0f))};

	auto toFar{forwardVector * m_shadowDistance};
	auto toNear{forwardVector * camera.GetNearPlane()};
	auto centreNear{toNear + camera.GetPosition()};
	auto centreFar{toFar + camera.GetPosition()};

	auto points{CalculateFrustumVertices(rotation, forwardVector, centreNear, centreFar)};

	m_minExtents = Vector3f::PositiveInfinity;
	m_maxExtents = Vector3f::NegativeInfinity;

	for (const auto &point : points)
	{
		Vector3f extent{point};
		m_minExtents = m_minExtents.Min(extent);
		m_maxExtents = m_minExtents.Max(extent);
	}

	m_maxExtents.m_z += m_shadowOffset;
}

void ShadowBox::UpdateSizes(const Camera &camera)
{
	m_farWidth = m_shadowDistance * std::tan(camera.GetFieldOfView());
	m_nearWidth = camera.GetNearPlane() * std::tan(camera.GetFieldOfView());
	m_farHeight = m_farWidth / Window::Get()->GetAspectRatio();
	m_nearHeight = m_nearWidth / Window::Get()->GetAspectRatio();
}

std::array<Vector4f, 8> ShadowBox::CalculateFrustumVertices(const Matrix4 &rotation, const Vector3f &forwardVector, const Vector3f &centreNear, const Vector3f &centreFar) const
{
	Vector3f upVector{rotation.Transform(Vector4f(0.0f, 1.0f, 0.0f, 0.0f))};
	auto rightVector{forwardVector.Cross(upVector)};
	auto downVector{-upVector};
	auto leftVector{-rightVector};

	auto farTop{centreFar + (upVector * m_farHeight)};
	auto farBottom{centreFar + (downVector * m_farHeight)};
	auto nearTop{centreNear + (upVector * m_nearHeight)};
	auto nearBottom{centreNear + (downVector * m_nearHeight)};

	std::array<Vector4f, 8> points;
	points[0] = CalculateLightSpaceFrustumCorner(farTop, rightVector, m_farWidth);
	points[1] = CalculateLightSpaceFrustumCorner(farTop, leftVector, m_farWidth);
	points[2] = CalculateLightSpaceFrustumCorner(farBottom, rightVector, m_farWidth);
	points[3] = CalculateLightSpaceFrustumCorner(farBottom, leftVector, m_farWidth);
	points[4] = CalculateLightSpaceFrustumCorner(nearTop, rightVector, m_nearWidth);
	points[5] = CalculateLightSpaceFrustumCorner(nearTop, leftVector, m_nearWidth);
	points[6] = CalculateLightSpaceFrustumCorner(nearBottom, rightVector, m_nearWidth);
	points[7] = CalculateLightSpaceFrustumCorner(nearBottom, leftVector, m_nearWidth);
	return points;
}

Vector4f ShadowBox::CalculateLightSpaceFrustumCorner(const Vector3f &startPoint, const Vector3f &direction, const float &width) const
{
	Vector4f point{startPoint + (direction * width)};
	point = m_lightViewMatrix.Transform(point);
	return point;
}

void ShadowBox::UpdateOrthoProjectionMatrix()
{
	m_projectionMatrix = Matrix4{};
	m_projectionMatrix[0][0] = 2.0f / GetWidth();
	m_projectionMatrix[1][1] = 2.0f / GetHeight();
	m_projectionMatrix[2][2] = -2.0f / GetDepth();
	m_projectionMatrix[3][3] = 1.0f;
}

void ShadowBox::UpdateCenter()
{
	Vector4f centre{(m_minExtents + m_maxExtents) / 2.0f};
	auto invertedLight{m_lightViewMatrix.Inverse()};
	m_centre = Vector3f(invertedLight.Transform(centre));
}

void ShadowBox::UpdateLightViewMatrix()
{
	m_lightViewMatrix = Matrix4{};
	auto pitch{std::acos(Vector2f{m_lightDirection.m_x, m_lightDirection.m_z}.Length())};
	m_lightViewMatrix = m_lightViewMatrix.Rotate(pitch, Vector3f::Right);
	auto yaw{std::atan(m_lightDirection.m_x / m_lightDirection.m_z)};

	if (m_lightDirection.m_z > 0.0f)
	{
		yaw -= Maths::Pi<float>;
	}

	m_lightViewMatrix = m_lightViewMatrix.Rotate(-yaw, Vector3f::Up);
	m_lightViewMatrix = m_lightViewMatrix.Translate(m_centre);
}

void ShadowBox::UpdateViewShadowMatrix()
{
	m_projectionViewMatrix = m_projectionMatrix * m_lightViewMatrix;
	m_shadowMapSpaceMatrix = m_offset * m_projectionViewMatrix;
}
}
