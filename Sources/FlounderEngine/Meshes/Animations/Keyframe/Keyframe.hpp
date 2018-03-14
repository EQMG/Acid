#pragma once

#include <map>
#include "../Joint/JointTransform.hpp"

namespace Flounder
{
	class F_EXPORT KeyframeData
	{
	private:
		float m_time;
		std::vector<JointTransformData*> *m_jointTransforms;
	public:
		KeyframeData(const float &time) :
			m_time(time),
			m_jointTransforms(new std::vector<JointTransformData*>())
		{
		}

		~KeyframeData()
		{
			for (auto joint : *m_jointTransforms)
			{
				delete joint;
			}

			delete m_jointTransforms;
		}

		float GetTime() const { return m_time; }

		std::vector<JointTransformData*> *GetJointTransforms() const { return m_jointTransforms; }

		void AddJointTransform(JointTransformData *transform)
		{
			m_jointTransforms->push_back(transform);
		}
	};

	class F_EXPORT Keyframe
	{
	private:
		float m_timeStamp;
		std::map<std::string, JointTransform*> *m_pose;
	public:
		Keyframe(const float &timeStamp, const std::map<std::string, JointTransform*> &pose);

		Keyframe(const KeyframeData &data);

		~Keyframe();

		float GetTimeStamp() const { return m_timeStamp; }

		std::map<std::string, JointTransform*> *GetPose() const { return m_pose; }
	};
}
