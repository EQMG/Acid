#include "PointEmitter.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
PointEmitter::PointEmitter() {
}

Vector3f PointEmitter::GeneratePosition() const {
	return point;
}

const Node &operator>>(const Node &node, PointEmitter &emitter) {
	node["point"].Get(emitter.point);
	return node;
}

Node &operator<<(Node &node, const PointEmitter &emitter) {
	node["point"].Set(emitter.point);
	return node;
}
}
