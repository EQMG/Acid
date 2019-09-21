#include "Animation.hpp"

namespace acid {
Animation::Animation(const Time &length, std::vector<Keyframe> keyframes) :
	m_length(length),
	m_keyframes(std::move(keyframes)) {
}

const Node &operator>>(const Node &node, Animation &animation) {
	node["length"].Get(animation.m_length);
	node["keyframes"].Get(animation.m_keyframes);
	return node;
}

Node &operator<<(Node &node, const Animation &animation) {
	node["length"].Set(animation.m_length);
	node["keyframes"].Set(animation.m_keyframes);
	return node;
}
}
