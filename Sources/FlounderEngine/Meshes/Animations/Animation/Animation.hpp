#pragma once

#include <vector>
#include "../Keyframe/Keyframe.hpp"

namespace Flounder
{
	class F_EXPORT AnimationData
	{
	private:
		float m_lengthSeconds;
		std::vector<KeyframeData*> m_keyframes;
	public:
		AnimationData(const float &lengthSeconds, const std::vector<KeyframeData*> &keyframes) :
			m_lengthSeconds(lengthSeconds),
			m_keyframes(std::vector<KeyframeData*>(keyframes))
		{
		}

		~AnimationData()
		{
			for (auto keyframeData : m_keyframes)
			{
				delete keyframeData;
			}
		}

		float GetLengthSeconds() const { return m_lengthSeconds; }

		std::vector<KeyframeData*> GetKeyframes() const { return m_keyframes; }
	};

	class F_EXPORT Animation
	{
	private:
		float m_length;

		std::vector<Keyframe*> *m_keyframes;
	public:
		Animation(const float &length, const std::vector<Keyframe*> &keyframes);

		Animation(const AnimationData &data);

		~Animation();

		float GetLength() const { return m_length; }

		std::vector<Keyframe*> *GetKeyframes() const { return m_keyframes; }
	};
}
