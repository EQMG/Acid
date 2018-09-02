#include "KeyframeData.hpp"

namespace acid
{
	KeyframeData::KeyframeData(const float &time) :
		m_time(time),
		m_jointTransforms(std::vector<JointTransformData>())
	{
	}

	KeyframeData::~KeyframeData()
	{
	}

	void KeyframeData::AddJointTransform(const JointTransformData &transform)
	{
		m_jointTransforms.emplace_back(transform);
	}
}
