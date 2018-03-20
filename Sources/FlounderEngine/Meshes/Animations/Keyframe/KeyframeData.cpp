#include "KeyframeData.hpp"

namespace Flounder
{
	KeyframeData::KeyframeData(const float &time) :
		m_time(time),
		m_jointTransforms(std::vector<JointTransformData *>())
	{
	}

	KeyframeData::~KeyframeData()
	{
	}

	void KeyframeData::AddJointTransform(JointTransformData *transform)
	{
		m_jointTransforms.push_back(transform);
	}
}
