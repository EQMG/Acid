#include "Animation.hpp"

namespace acid
{
Animation::Animation(const Time &length, std::vector<Keyframe> keyframes) :
	m_length(length),
	m_keyframes(std::move(keyframes))
{
}

const Node &operator>>(const Node &node, Animation &animation)
{
	node.GetChild("length", animation.m_length);
	node.GetChild("keyframes", animation.m_keyframes);
	return node;
}

Node &operator<<(Node &node, const Animation &animation)
{
	node.SetChild("length", animation.m_length);
	node.SetChild("keyframes", animation.m_keyframes);
	return node;
}
}
