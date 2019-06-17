#include "Animation.hpp"

namespace acid
{
Animation::Animation(const Time &length, std::vector<Keyframe> keyframes) :
	m_length{length},
	m_keyframes{std::move(keyframes)}
{
}

const Metadata &operator>>(const Metadata &metadata, Animation &animation)
{
	metadata.GetChild("length", animation.m_length);
	metadata.GetChild("keyframes", animation.m_keyframes);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const Animation &animation)
{
	metadata.SetChild("length", animation.m_length);
	metadata.SetChild("keyframes", animation.m_keyframes);
	return metadata;
}
}
