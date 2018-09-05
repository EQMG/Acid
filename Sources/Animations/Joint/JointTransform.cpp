#include "JointTransform.hpp"

namespace acid
{
	JointTransform::JointTransform(const Vector3 &position, const Quaternion &rotation) :
		m_position(position),
		m_rotation(rotation)
	{
	}

	JointTransform::JointTransform(const Matrix4 &localTransform) :
		m_position(Vector3(localTransform[3])),
		m_rotation(Quaternion(localTransform))
	{
	}

	JointTransform::JointTransform(const JointTransformData &data) :
		m_position(Vector3(data.GetJointLocalTransform()[3])),
		m_rotation(Quaternion(data.GetJointLocalTransform()))
	{
	}

	JointTransform::~JointTransform()
	{
	}

	Matrix4 JointTransform::GetLocalTransform() const
	{
		Matrix4 matrix = Matrix4();
		matrix = matrix.Translate(m_position);
		matrix = matrix * m_rotation.ToRotationMatrix();
		return matrix;
	}

	JointTransform JointTransform::Interpolate(const JointTransform &frameA, const JointTransform &frameB, const float &progression)
	{
		Vector3 pos = Interpolate(frameA.GetPosition(), frameB.GetPosition(), progression);
		Quaternion rot = frameA.GetRotation().Slerp(frameB.GetRotation(), progression);
		return JointTransform(pos, rot);
	}

	Vector3 JointTransform::Interpolate(const Vector3 &start, const Vector3 &end, const float &progression)
	{
		float x = start.m_x + (end.m_x - start.m_x) * progression;
		float y = start.m_y + (end.m_y - start.m_y) * progression;
		float z = start.m_z + (end.m_z - start.m_z) * progression;
		return Vector3(x, y, z);
	}
}
