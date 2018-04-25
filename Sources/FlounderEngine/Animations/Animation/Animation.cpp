#include "Animation.hpp"

namespace fl
{
	Animation::Animation(const float &length, const std::vector<Keyframe *> &keyframes) :
		m_length(length),
		m_keyframes(new std::vector<Keyframe *>(keyframes))
	{
	}

	Animation::Animation(const float &length, const std::vector<KeyframeData *> &keyframeData) :
		m_length(length),
		m_keyframes(new std::vector<Keyframe *>())
	{
		for (auto frameData : keyframeData)
		{
			m_keyframes->push_back(new Keyframe(*frameData));
		}
	}

	Animation::~Animation()
	{
		for (auto keyFrame : *m_keyframes)
		{
			delete keyFrame;
		}

		delete m_keyframes;
	}
}
