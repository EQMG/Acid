#include "JointTransform.hpp"

namespace fl
{
	JointTransform::JointTransform(const Vector3 &position, const Quaternion &rotation) :
		m_position(Vector3(position)),
		m_rotation(Quaternion(rotation))
	{
	}

	JointTransform::JointTransform(const Matrix4 &localTransform) :
		m_position(Vector3(localTransform.m_30, localTransform.m_31, localTransform.m_32)),
		m_rotation(Quaternion(localTransform))
	{
	}

	JointTransform::JointTransform(const JointTransformData &data) :
		m_position(Vector3(data.GetJointLocalTransform().m_30, data.GetJointLocalTransform().m_31, data.GetJointLocalTransform().m_32)),
		m_rotation(Quaternion(data.GetJointLocalTransform()))
	{
	}

	JointTransform::~JointTransform()
	{
	}

	Matrix4 JointTransform::GetLocalTransform()
	{
		Matrix4 rotationMatrix = m_rotation.ToRotationMatrix();
		Matrix4 matrix = Matrix4();
		matrix = matrix.Translate(m_position);
		matrix *= rotationMatrix;
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
