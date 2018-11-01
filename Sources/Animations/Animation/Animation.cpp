#include "Animation.hpp"

namespace acid
{
	Animation::Animation(const Time &length, const std::vector<Keyframe> &keyframes) :
		m_length(length),
		m_keyframes(keyframes)
	{
	}

	Animation::Animation(const Time &length, const std::vector<KeyframeData> &keyframeData) :
		m_length(length),
		m_keyframes(std::vector<Keyframe>())
	{
		for (auto &frameData : keyframeData)
		{
			m_keyframes.emplace_back(Keyframe(frameData));
		}
	}
}
