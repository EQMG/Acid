#include "Animation.hpp"

namespace Flounder
{
	Animation::Animation(const float &length, const std::vector<Keyframe *> &keyframes) :
		m_length(length),
		m_keyframes(new std::vector<Keyframe*>(keyframes))
	{
	}

	Animation::Animation(const AnimationData &data) :
		m_length(data.GetLengthSeconds()),
		m_keyframes(new std::vector<Keyframe*>())
	{
		for (auto frameData : *data.GetKeyframes())
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
