#include "ShadowBox.hpp"

#include "Renderer/Renderer.hpp"
#include "Maths/Maths.hpp"

namespace acid
{
	ShadowBox::ShadowBox() :
		m_shadowOffset(0.0f),
		m_shadowDistance(0.0f),
		m_farHeight(0.0f),
		m_farWidth(0.0f),
		m_nearHeight(0.0f),
		m_nearWidth(0.0f)
	{
		// Creates the offset for part of the conversion to shadow map space.
		m_offset = m_offset.Translate(Vector3(0.5f, 0.5f, 0.5f));
		m_offset = m_offset.Scale(Vector3(0.5f, 0.5f, 0.5f));
	}

	void ShadowBox::Update(const Camera &camera, const Vector3 &lightPosition, const float &shadowOffset, const float &shadowDistance)
	{
		m_lightDirection = lightPosition.Normalize();
		m_shadowOffset = shadowOffset;
		m_shadowDistance = shadowDistance;

		UpdateShadowBox(camera);
		UpdateOrthoProjectionMatrix();
		UpdateCenter();
		UpdateLightViewMatrix();
		UpdateViewShadowMatrix();
	}

	bool ShadowBox::IsInBox(const Vector3 &position, const float &radius) const
	{
		auto entityPos = m_lightViewMatrix.Transform(Vector4(position));

		auto closestPoint = Vector3();
		closestPoint.m_x = std::clamp(entityPos.m_x, m_minExtents.m_x, m_maxExtents.m_x);
		closestPoint.m_y = std::clamp(entityPos.m_y, m_minExtents.m_y, m_maxExtents.m_y);
		closestPoint.m_z = std::clamp(entityPos.m_z, m_minExtents.m_z, m_maxExtents.m_z);

		auto centre = Vector3(entityPos);
		auto distance = centre - closestPoint;
		auto distanceSquared = distance.LengthSquared();

		return distanceSquared < radius * radius;
	}

	void ShadowBox::UpdateShadowBox(const Camera &camera)
	{
		UpdateSizes(camera);

		auto rotation = Matrix4();
		rotation = rotation.Rotate(camera.GetRotation().m_y * Maths::DegToRad, Vector3::Up);
		rotation = rotation.Rotate(camera.GetRotation().m_x * Maths::DegToRad, Vector3::Right);

		auto forwardVector4 = rotation.Transform(Vector4(0.0f, 0.0f, -1.0f, 0.0f));
		auto forwardVector = Vector3(forwardVector4);

		auto toFar = forwardVector * m_shadowDistance;
		auto toNear = forwardVector * camera.GetNearPlane();
		auto centreNear = toNear + camera.GetPosition();
		auto centreFar = toFar + camera.GetPosition();

		auto points = CalculateFrustumVertices(rotation, forwardVector, centreNear, centreFar);

		for (uint32_t i = 0; i < 8; i++)
		{
			auto point = points[i];

			if (i == 0)
			{
				m_minExtents.m_x = point.m_x;
				m_maxExtents.m_x = point.m_x;
				m_minExtents.m_y = point.m_y;
				m_maxExtents.m_y = point.m_y;
				m_minExtents.m_z = point.m_z;
				m_maxExtents.m_z = point.m_z;
			}
			else
			{
				if (point.m_x > m_maxExtents.m_x)
				{
					m_maxExtents.m_x = point.m_x;
				}
				else if (point.m_x < m_minExtents.m_x)
				{
					m_minExtents.m_x = point.m_x;
				}

				if (point.m_y > m_maxExtents.m_y)
				{
					m_maxExtents.m_y = point.m_y;
				}
				else if (point.m_y < m_minExtents.m_y)
				{
					m_minExtents.m_y = point.m_y;
				}

				if (point.m_z > m_maxExtents.m_z)
				{
					m_maxExtents.m_z = point.m_z;
				}
				else if (point.m_z < m_minExtents.m_z)
				{
					m_minExtents.m_z = point.m_z;
				}
			}
		}

		m_maxExtents.m_z += m_shadowOffset;
	}

	void ShadowBox::UpdateSizes(const Camera &camera)
	{
		m_farWidth = m_shadowDistance * std::tan(camera.GetFieldOfView() * Maths::DegToRad);
		m_nearWidth = camera.GetNearPlane() * std::tan(camera.GetFieldOfView() * Maths::DegToRad);
		m_farHeight = m_farWidth / Window::Get()->GetAspectRatio();
		m_nearHeight = m_nearWidth / Window::Get()->GetAspectRatio();
	}

	std::array<Vector4, 8> ShadowBox::CalculateFrustumVertices(const Matrix4 &rotation, const Vector3 &forwardVector, const Vector3 &centreNear, const Vector3 &centreFar)
	{
		auto upVector4 = rotation.Transform(Vector4(0.0f, 1.0f, 0.0f, 0.0f));
		auto upVector = Vector3(upVector4);
		auto rightVector = forwardVector.Cross(upVector);
		auto downVector = -upVector;
		auto leftVector = -rightVector;
		
		auto farUpVector = upVector * m_farHeight;
		auto farDownVector = downVector * m_farHeight;
		auto nearUpVector = upVector * m_nearHeight;
		auto nearDownVector = downVector * m_nearHeight;
		
		auto farTop = centreFar + farUpVector;
		auto farBottom = centreFar + farDownVector;
		auto nearTop = centreNear + nearUpVector;
		auto nearBottom = centreNear + nearDownVector;

		auto points = std::array<Vector4, 8>();
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

	Vector4 ShadowBox::CalculateLightSpaceFrustumCorner(const Vector3 &startPoint, const Vector3 &direction, const float &width) const
	{
		auto point = startPoint + (direction * width);
		auto point4 = Vector4(point);
		point4 = m_lightViewMatrix.Transform(point4);
		return point4;
	}

	void ShadowBox::UpdateOrthoProjectionMatrix()
	{
		m_projectionMatrix = Matrix4::Identity;
		m_projectionMatrix[0][0] = 2.0f / GetWidth();
		m_projectionMatrix[1][1] = 2.0f / GetHeight();
		m_projectionMatrix[2][2] = -2.0f / GetDepth();
		m_projectionMatrix[3][3] = 1.0f;
	}

	void ShadowBox::UpdateCenter()
	{
		auto x = (m_minExtents.m_x + m_maxExtents.m_x) / 2.0f;
		auto y = (m_minExtents.m_y + m_maxExtents.m_y) / 2.0f;
		auto z = (m_minExtents.m_z + m_maxExtents.m_z) / 2.0f;
		auto centre = Vector4(x, y, z, 1.0f);
		auto invertedLight = m_lightViewMatrix.Inverse();

		m_centre = invertedLight.Transform(centre);
	}

	void ShadowBox::UpdateLightViewMatrix()
	{
		m_lightViewMatrix = Matrix4::Identity;
		auto pitch = std::acos(Vector2(m_lightDirection.m_x, m_lightDirection.m_z).Length());
		m_lightViewMatrix = m_lightViewMatrix.Rotate(pitch, Vector3::Right);
		auto yaw = std::atan(m_lightDirection.m_x / m_lightDirection.m_z) * Maths::RadToDeg;

		if (m_lightDirection.m_z > 0.0f)
		{
			yaw -= 180.0f;
		}

		m_lightViewMatrix = m_lightViewMatrix.Rotate(-yaw * Maths::DegToRad, Vector3::Up);
		m_lightViewMatrix = m_lightViewMatrix.Translate(-m_centre);
	}

	void ShadowBox::UpdateViewShadowMatrix()
	{
		m_projectionViewMatrix = m_projectionMatrix * m_lightViewMatrix;
		m_shadowMapSpaceMatrix = m_offset * m_projectionViewMatrix;
	}
}
