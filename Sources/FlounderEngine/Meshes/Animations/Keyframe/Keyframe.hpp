#pragma once

#include <map>
#include "../Joint/JointTransform.hpp"
#include "KeyframeData.hpp"

namespace Flounder
{
	class F_EXPORT Keyframe
	{
	private:
		float m_timeStamp;
		std::map<std::string, JointTransform *> *m_pose;
	public:
		Keyframe(const float &timeStamp, const std::map<std::string, JointTransform *> &pose);

		Keyframe(const KeyframeData &data);

		~Keyframe();

		float GetTimeStamp() const { return m_timeStamp; }

		std::map<std::string, JointTransform *> *GetPose() const { return m_pose; }
	};
}
