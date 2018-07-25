#include "ShadowBox.hpp"

#include "Display/Display.hpp"
#include "Maths/Maths.hpp"

namespace acid
{
	ShadowBox::ShadowBox() :
		m_lightDirection(Vector3()),
		m_shadowOffset(0.0f),
		m_shadowDistance(0.0f),
		m_projectionMatrix(Matrix4()),
		m_lightViewMatrix(Matrix4()),
		m_projectionViewMatrix(Matrix4()),
		m_shadowMapSpaceMatrix(Matrix4()),
		m_offset(CreateOffset()),
		m_centre(Vector3()),
		m_farHeight(0.0f),
		m_farWidth(0.0f),
		m_nearHeight(0.0f),
		m_nearWidth(0.0f),
		m_minExtents(Vector3()),
		m_maxExtents(Vector3())
	{
	}

	ShadowBox::~ShadowBox()
	{
	}

	void ShadowBox::Update(const ICamera &camera, const Vector3 &lightPosition, const float &shadowOffset, const float &shadowDistance)
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

	Matrix4 ShadowBox::CreateOffset()
	{
		Matrix4 offset = Matrix4();
		offset = offset.Translate(Vector3(0.5f, 0.5f, 0.5f));
		offset = offset.Scale(Vector3(0.5f, 0.5f, 0.5f));
		return offset;
	}

	void ShadowBox::UpdateShadowBox(const ICamera &camera)
	{
		UpdateSizes(camera);

		Matrix4 rotation = Matrix4();
		rotation = rotation.Rotate(Maths::Radians(camera.GetRotation().m_y), Vector3::UP);
		rotation = rotation.Rotate(Maths::Radians(camera.GetRotation().m_x), Vector3::RIGHT);

		Vector4 forwardVector4 = rotation.Transform(Vector4(0.0f, 0.0f, -1.0f, 0.0f));
		Vector3 forwardVector = Vector3(forwardVector4);

		Vector3 toFar = forwardVector * m_shadowDistance;
		Vector3 toNear = forwardVector * camera.GetNearPlane();
		Vector3 centreNear = toNear + camera.GetPosition();
		Vector3 centreFar = toFar + camera.GetPosition();

		auto points = CalculateFrustumVertices(rotation, forwardVector, centreNear, centreFar);

		for (int i = 0; i < 8; i++)
		{
			Vector4 point = points[i];

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

	void ShadowBox::UpdateSizes(const ICamera &camera)
	{
		m_farWidth = m_shadowDistance * std::tan(Maths::Radians(camera.GetFov()));
		m_nearWidth = camera.GetNearPlane() * std::tan(Maths::Radians(camera.GetFov()));
		m_farHeight = m_farWidth / Display::Get()->GetAspectRatio();
		m_nearHeight = m_nearWidth / Display::Get()->GetAspectRatio();
	}

	std::array<Vector4, 8> ShadowBox::CalculateFrustumVertices(const Matrix4 &rotation, const Vector3 &forwardVector, const Vector3 &centreNear, const Vector3 &centreFar)
	{
		Vector4 upVector4 = rotation.Transform(Vector4(0.0f, 1.0f, 0.0f, 0.0f));
		Vector3 upVector = Vector3(upVector4);
		Vector3 rightVector = forwardVector.Cross(upVector);
		Vector3 downVector = -upVector;
		Vector3 leftVector = -rightVector;

		Vector3 farUpVector = upVector * m_farHeight;
		Vector3 farDownVector = downVector * m_farHeight;
		Vector3 nearUpVector = upVector * m_nearHeight;
		Vector3 nearDownVector = downVector * m_nearHeight;

		Vector3 farTop = centreFar + farUpVector;
		Vector3 farBottom = centreFar + farDownVector;
		Vector3 nearTop = centreNear + nearUpVector;
		Vector3 nearBottom = centreNear + nearDownVector;

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

	Vector4 ShadowBox::CalculateLightSpaceFrustumCorner(const Vector3 &startPoint, const Vector3 &direction, const float &width)
	{
		Vector3 point = startPoint + (direction * width);
		Vector4 point4 = Vector4(point);
		point4 = m_lightViewMatrix.Transform(point4);
		return point4;
	}

	void ShadowBox::UpdateOrthoProjectionMatrix()
	{
		m_projectionMatrix = Matrix4::IDENTITY;
		m_projectionMatrix[0][0] = 2.0f / GetWidth();
		m_projectionMatrix[1][1] = 2.0f / GetHeight();
		m_projectionMatrix[2][2] = -2.0f / GetDepth();
		m_projectionMatrix[3][3] = 1.0f;
	}

	void ShadowBox::UpdateCenter()
	{
		float x = (m_minExtents.m_x + m_maxExtents.m_x) / 2.0f;
		float y = (m_minExtents.m_y + m_maxExtents.m_y) / 2.0f;
		float z = (m_minExtents.m_z + m_maxExtents.m_z) / 2.0f;
		Vector4 centre = Vector4(x, y, z, 1.0f);
		Matrix4 invertedLight = m_lightViewMatrix.Invert();

		m_centre = invertedLight.Transform(centre);
	}

	void ShadowBox::UpdateLightViewMatrix()
	{
		m_lightViewMatrix = Matrix4::IDENTITY;
		float pitch = std::acos(Vector2(m_lightDirection.m_x, m_lightDirection.m_z).Length());
		m_lightViewMatrix = m_lightViewMatrix.Rotate(pitch, Vector3::RIGHT);
		float yaw = Maths::Degrees(std::atan(m_lightDirection.m_x / m_lightDirection.m_z));

		if (m_lightDirection.m_z > 0.0f)
		{
			yaw -= 180.0f;
		}

		m_lightViewMatrix = m_lightViewMatrix.Rotate(-Maths::Radians(yaw), Vector3::UP);
		m_lightViewMatrix = m_lightViewMatrix.Translate(-m_centre);
	}

	void ShadowBox::UpdateViewShadowMatrix()
	{
		m_projectionViewMatrix = Matrix4::IDENTITY;
		m_shadowMapSpaceMatrix = Matrix4::IDENTITY;
		m_projectionViewMatrix = m_projectionMatrix * m_lightViewMatrix;
		m_shadowMapSpaceMatrix = m_offset * m_projectionViewMatrix;
	}

	bool ShadowBox::IsInBox(const Vector3 &position, const float &radius) const
	{
		Vector4 entityPos = m_lightViewMatrix.Transform(Vector4(position));

		Vector3 closestPoint = Vector3();
		closestPoint.m_x = Maths::Clamp(entityPos.m_x, m_minExtents.m_x, m_maxExtents.m_x);
		closestPoint.m_y = Maths::Clamp(entityPos.m_y, m_minExtents.m_y, m_maxExtents.m_y);
		closestPoint.m_z = Maths::Clamp(entityPos.m_z, m_minExtents.m_z, m_maxExtents.m_z);

		Vector3 centre = Vector3(entityPos);
		Vector3 distance = centre - closestPoint;
		float distanceSquared = distance.LengthSquared();

		return distanceSquared < radius * radius;
	}
}
