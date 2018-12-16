#include "Keyframe.hpp"

namespace acid
{
	Keyframe::Keyframe(const Time &timeStamp, const std::map<std::string, JointTransform> &pose) :
		m_timeStamp(timeStamp),
		m_pose(pose)
	{
	}
}
