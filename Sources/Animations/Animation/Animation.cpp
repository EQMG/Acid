#include "Animation.hpp"

namespace acid
{
	Animation::Animation(const Time &length, const std::vector<Keyframe> &keyframes) :
		m_length(length),
		m_keyframes(keyframes)
	{
	}
}
