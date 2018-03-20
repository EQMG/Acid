#include "JointTransformData.hpp"

namespace Flounder
{
	JointTransformData::JointTransformData(const std::string &jointNameId, const Matrix4 &jointLocalTransform) :
		m_jointNameId(jointNameId),
		m_jointLocalTransform(Matrix4(jointLocalTransform))
	{
	}

	JointTransformData::~JointTransformData()
	{
	}
}
