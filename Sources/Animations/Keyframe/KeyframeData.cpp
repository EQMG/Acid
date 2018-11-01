#include "KeyframeData.hpp"

namespace acid
{
	KeyframeData::KeyframeData(const Time &time) :
		m_time(time),
		m_jointTransforms(std::vector<JointTransformData>())
	{
	}

	void KeyframeData::AddJointTransform(const JointTransformData &transform)
	{
		m_jointTransforms.emplace_back(transform);
	}
}
