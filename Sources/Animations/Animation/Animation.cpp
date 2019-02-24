#include "Animation.hpp"

#include <utility>

namespace acid
{
	Animation::Animation(const Time &length, std::vector<Keyframe> keyframes) :
		m_length(length),
		m_keyframes(std::move(keyframes))
	{
	}
}
