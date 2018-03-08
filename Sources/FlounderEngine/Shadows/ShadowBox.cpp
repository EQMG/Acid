#include "ShadowBox.hpp"

#include "../Devices/Display.hpp"
#include "../Maths/Maths.hpp"

namespace Flounder
{
	ShadowBox::ShadowBox() :
		m_lightDirection(new Vector3()),
		m_shadowOffset(0.0f),
		m_shadowDistance(0.0f),
		m_projectionMatrix(new Matrix4()),
		m_lightViewMatrix(new Matrix4()),
		m_projectionViewMatrix(new Matrix4()),
		m_shadowMapSpaceMatrix(new Matrix4()),
		m_offset(CreateOffset()),
		m_centre(new Vector3()),
		m_farHeight(0.0f),
		m_farWidth(0.0f),
		m_nearHeight(0.0f),
		m_nearWidth(0.0f),
		m_aabb(new ColliderAabb())
	{
	}

	ShadowBox::~ShadowBox()
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

	void ShadowBox::Update(const ICamera &camera, const Vector3 &lightPosition, const float &shadowOffset, const float &shadowDistance)
	{
		m_lightDirection->Set(lightPosition);
		m_lightDirection->Normalize();
		m_shadowOffset = shadowOffset;
		m_shadowDistance = shadowDistance;

		UpdateShadowBox(camera);
		UpdateOrthoProjectionMatrix();
		UpdateCenter();
		UpdateLightViewMatrix();
		UpdateViewShadowMatrix();
	}

	Matrix4 *ShadowBox::CreateOffset()
	{
		Matrix4 *offset = new Matrix4();
		Matrix4::Translate(*offset, Vector3(0.5f, 0.5f, 0.5f), offset);
		Matrix4::Scale(*offset, Vector3(0.5f, 0.5f, 0.5f), offset);
		return offset;
	}

	void ShadowBox::UpdateShadowBox(const ICamera &camera)
	{
		UpdateSizes(camera);

		Matrix4 *rotation = new Matrix4();
		Matrix4::Rotate(*rotation, Vector3::UP, Maths::Radians(camera.GetRotation()->m_y), rotation);
		Matrix4::Rotate(*rotation, Vector3::RIGHT, Maths::Radians(camera.GetRotation()->m_x), rotation);

		Vector4 *forwardVector4 = Matrix4::Transform(*rotation, Vector4(0.0f, 0.0f, -1.0f, 0.0f), nullptr);
		Vector3 *forwardVector = new Vector3(*forwardVector4);

		Vector3 *toFar = new Vector3(*forwardVector);
		toFar->Scale(m_shadowDistance);
		Vector3 *toNear = new Vector3(*forwardVector);
		toNear->Scale(camera.GetNearPlane());
		Vector3 *centreNear = Vector3::Add(*toNear, *camera.GetPosition(), nullptr);
		Vector3 *centreFar = Vector3::Add(*toFar, *camera.GetPosition(), nullptr);

		Vector4 **points = CalculateFrustumVertices(*rotation, *forwardVector, *centreNear, *centreFar);

		for (int i = 0; i < 8; i++)
		{
			Vector4 *point = points[i];

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

	void ShadowBox::UpdateSizes(const ICamera &camera)
	{
		m_farWidth = m_shadowDistance * std::tan(Maths::Radians(camera.GetFov()));
		m_nearWidth = camera.GetNearPlane() * std::tan(Maths::Radians(camera.GetFov()));
		m_farHeight = m_farWidth / Display::Get()->GetAspectRatio();
		m_nearHeight = m_nearWidth / Display::Get()->GetAspectRatio();
	}

	Vector4 **ShadowBox::CalculateFrustumVertices(const Matrix4 &rotation, const Vector3 &forwardVector, const Vector3 &centreNear, const Vector3 &centreFar) const
	{
		Vector4 *upVector4 = Matrix4::Transform(rotation, Vector4(0.0f, 1.0f, 0.0f, 0.0f), nullptr);
		Vector3 *upVector = new Vector3(*upVector4);
		Vector3 *rightVector = Vector3::Cross(forwardVector, *upVector, nullptr);
		Vector3 *downVector = new Vector3(*upVector);
		downVector->Negate();
		Vector3 *leftVector = new Vector3(*rightVector);
		leftVector->Negate();

		Vector3 *farUpVector = new Vector3(*upVector);
		farUpVector->Scale(m_farHeight);
		Vector3 *farDownVector = new Vector3(*downVector);
		farDownVector->Scale(m_farHeight);
		Vector3 *nearUpVector = new Vector3(*upVector);
		nearUpVector->Scale(m_nearHeight);
		Vector3 *nearDownVector = new Vector3(*downVector);
		nearDownVector->Scale(m_nearHeight);

		Vector3 *farTop = Vector3::Add(centreFar, *farUpVector, nullptr);
		Vector3 *farBottom = Vector3::Add(centreFar, *farDownVector, nullptr);
		Vector3 *nearTop = Vector3::Add(centreNear, *nearUpVector, nullptr);
		Vector3 *nearBottom = Vector3::Add(centreNear, *nearDownVector, nullptr);

		Vector4 **points = new Vector4 *[8];
		points[0] = CalculateLightSpaceFrustumCorner(*farTop, *rightVector, m_farWidth);
		points[1] = CalculateLightSpaceFrustumCorner(*farTop, *leftVector, m_farWidth);
		points[2] = CalculateLightSpaceFrustumCorner(*farBottom, *rightVector, m_farWidth);
		points[3] = CalculateLightSpaceFrustumCorner(*farBottom, *leftVector, m_farWidth);
		points[4] = CalculateLightSpaceFrustumCorner(*nearTop, *rightVector, m_nearWidth);
		points[5] = CalculateLightSpaceFrustumCorner(*nearTop, *leftVector, m_nearWidth);
		points[6] = CalculateLightSpaceFrustumCorner(*nearBottom, *rightVector, m_nearWidth);
		points[7] = CalculateLightSpaceFrustumCorner(*nearBottom, *leftVector, m_nearWidth);

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

	Vector4 *ShadowBox::CalculateLightSpaceFrustumCorner(const Vector3 &startPoint, const Vector3 &direction, const float &width) const
	{
		Vector3 *point = Vector3::Add(startPoint, *Vector3(direction).Scale(width), nullptr);
		Vector4 *point4 = new Vector4(*point);
		Matrix4::Transform(*m_lightViewMatrix, *point4, point4);

		delete point;
		return point4;
	}

	void ShadowBox::UpdateOrthoProjectionMatrix() const
	{
		m_projectionMatrix->SetIdentity();
		m_projectionMatrix->m_00 = 2.0f / m_aabb->GetWidth();
		m_projectionMatrix->m_11 = 2.0f / m_aabb->GetHeight();
		m_projectionMatrix->m_22 = -2.0f / m_aabb->GetDepth();
		m_projectionMatrix->m_33 = 1.0f;
	}

	void ShadowBox::UpdateCenter() const
	{
		float x = (m_aabb->m_minExtents->m_x + m_aabb->m_maxExtents->m_x) / 2.0f;
		float y = (m_aabb->m_minExtents->m_y + m_aabb->m_maxExtents->m_y) / 2.0f;
		float z = (m_aabb->m_minExtents->m_z + m_aabb->m_maxExtents->m_z) / 2.0f;
		Vector4 centre = Vector4(x, y, z, 1.0f);
		Matrix4 *invertedLight = Matrix4::Invert(*m_lightViewMatrix, nullptr);
		Vector4 *centre4 = Matrix4::Transform(*invertedLight, centre, nullptr);

		m_centre->Set(*centre4);

		delete invertedLight;
		delete centre4;
	}

	void ShadowBox::UpdateLightViewMatrix() const
	{
		m_centre->Negate();

		m_lightViewMatrix->SetIdentity();
		float pitch = std::acos(Vector2(m_lightDirection->m_x, m_lightDirection->m_z).Length());
		Matrix4::Rotate(*m_lightViewMatrix, Vector3::RIGHT, pitch, m_lightViewMatrix);
		float yaw = Maths::Degrees(std::atan(m_lightDirection->m_x / m_lightDirection->m_z));

		if (m_lightDirection->m_z > 0.0f)
		{
			yaw -= 180.0f;
		}

		Matrix4::Rotate(*m_lightViewMatrix, Vector3::UP, -Maths::Radians(yaw), m_lightViewMatrix);
		Matrix4::Translate(*m_lightViewMatrix, *m_centre, m_lightViewMatrix);

		m_centre->Negate();
	}

	void ShadowBox::UpdateViewShadowMatrix() const
	{
		m_projectionViewMatrix->SetIdentity();
		m_shadowMapSpaceMatrix->SetIdentity();
		Matrix4::Multiply(*m_projectionMatrix, *m_lightViewMatrix, m_projectionViewMatrix);
		Matrix4::Multiply(*m_offset, *m_projectionViewMatrix, m_shadowMapSpaceMatrix);
	}

	bool ShadowBox::IsInBox(const Vector3 &position, const float &radius) const
	{
		Vector4 *entityPos = Matrix4::Transform(*m_lightViewMatrix, Vector4(position), nullptr);

		Vector3 *closestPoint = new Vector3();
		closestPoint->m_x = Maths::Clamp(entityPos->m_x, m_aabb->m_minExtents->m_x, m_aabb->m_maxExtents->m_x);
		closestPoint->m_y = Maths::Clamp(entityPos->m_y, m_aabb->m_minExtents->m_y, m_aabb->m_maxExtents->m_y);
		closestPoint->m_z = Maths::Clamp(entityPos->m_z, m_aabb->m_minExtents->m_z, m_aabb->m_maxExtents->m_z);

		Vector3 *centre = new Vector3(*entityPos);
		Vector3 *distance = Vector3::Subtract(*centre, *closestPoint, nullptr);
		float distanceSquared = distance->LengthSquared();

		delete distance;
		delete centre;
		delete closestPoint;
		delete entityPos;

		return distanceSquared < radius * radius;
	}
}
