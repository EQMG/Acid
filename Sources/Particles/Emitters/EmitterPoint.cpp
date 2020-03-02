#include "EmitterPoint.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
EmitterPoint::EmitterPoint() {
}

Vector3f EmitterPoint::GeneratePosition() const {
	return point;
}

const Node &operator>>(const Node &node, EmitterPoint &emitter) {
	node["point"].Get(emitter.point);
	return node;
}

Node &operator<<(Node &node, const EmitterPoint &emitter) {
	node["point"].Set(emitter.point);
	return node;
}
}
