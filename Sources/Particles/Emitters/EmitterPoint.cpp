#include "EmitterPoint.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
bool EmitterPoint::registered = Register("point");

EmitterPoint::EmitterPoint() {
}

Vector3f EmitterPoint::GeneratePosition() const {
	return m_point;
}

const Node &operator>>(const Node &node, EmitterPoint &emitter) {
	node["point"].Get(emitter.m_point);
	return node;
}

Node &operator<<(Node &node, const EmitterPoint &emitter) {
	node["point"].Set(emitter.m_point);
	return node;
}
}
