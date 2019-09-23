#include "EmitterPoint.hpp"

#include "Scenes/Entity.hpp"

namespace acid {
EmitterPoint::Registrar<EmitterPoint> EmitterPoint::registered("emitterPoint");

EmitterPoint::EmitterPoint() {
}

void EmitterPoint::Start() {
}

void EmitterPoint::Update() {
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
