#include "Animation.hpp"

namespace fl
{
	Animation::Animation(const float &length, const std::vector<Keyframe *> &keyframes) :
		m_length(length),
		m_keyframes(keyframes)
	{
	}

	Animation::Animation(const float &length, const std::vector<KeyframeData *> &keyframeData) :
		m_length(length),
		m_keyframes(std::vector<Keyframe *>())
	{
		for (auto &frameData : keyframeData)
		{
			m_keyframes.emplace_back(new Keyframe(*frameData));
		}
	}

	Animation::~Animation()
	{
		for (auto &keyFrame : m_keyframes)
		{
			delete keyFrame;
		}
	}
}
