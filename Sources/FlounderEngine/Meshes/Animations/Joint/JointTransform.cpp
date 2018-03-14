#include "JointTransform.hpp"

namespace Flounder
{
	JointTransform::JointTransform(const Vector3 &position, const Quaternion &rotation) :
		m_position(new Vector3(position)),
		m_rotation(new Quaternion(rotation))
	{
	}

	JointTransform::JointTransform(const JointTransformData &data) :
		m_position(new Vector3()),
		m_rotation(new Quaternion())
	{
		auto matrix = data.GetJointLocalTransform();
		m_position->Set(matrix.m_30, matrix.m_31, matrix.m_32);
		m_rotation->Set(matrix);
	}

	JointTransform::JointTransform(const Matrix4 &localTransform) :
		m_position(new Vector3(localTransform.m_30, localTransform.m_31, localTransform.m_32)),
		m_rotation(new Quaternion(localTransform))
	{
	}

	JointTransform::~JointTransform()
	{
		delete m_position;
		delete m_rotation;
	}

	Matrix4 *JointTransform::GetLocalTransform()
	{
		Matrix4 *rotationMatrix = Quaternion::ToRotationMatrix(*m_rotation, nullptr);
		Matrix4 *matrix = new Matrix4();
		Matrix4::Translate(*matrix, *m_position, matrix);
		Matrix4::Multiply(*matrix, *rotationMatrix, matrix);
		delete rotationMatrix;
		return matrix;
	}

	JointTransform *JointTransform::Interpolate(const JointTransform &frameA, const JointTransform &frameB, const float &progression)
	{
		Vector3 pos = Interpolate(*frameA.GetPosition(), *frameB.GetPosition(), progression);
		Quaternion rot = Quaternion();
		Quaternion::Slerp(*frameA.GetRotation(), *frameB.GetRotation(), progression, &rot);
		return new JointTransform(pos, rot);
	}

	Vector3 JointTransform::Interpolate(const Vector3 &start, const Vector3 &end, const float &progression)
	{
		float x = start.m_x + (end.m_x - start.m_x) * progression;
		float y = start.m_y + (end.m_y - start.m_y) * progression;
		float z = start.m_z + (end.m_z - start.m_z) * progression;
		return Vector3(x, y, z);
	}
}
