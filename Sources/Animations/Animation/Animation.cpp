#include "Animation.hpp"

namespace acid
{
Animation::Animation(const Time &length, std::vector<Keyframe> keyframes) :
	m_length(length),
	m_keyframes(std::move(keyframes))
{
}
}
