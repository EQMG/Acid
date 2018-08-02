#include "KeyframeData.hpp"

namespace acid
{
	KeyframeData::KeyframeData(const float &time) :
		m_time(time)
	{
	}

	KeyframeData::~KeyframeData()
	{
	}

	void KeyframeData::AddJointTransform(JointTransformData *transform)
	{
		m_jointTransforms.emplace_back(transform);
	}
}
