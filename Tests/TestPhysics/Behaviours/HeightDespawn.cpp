#include "HeightDespawn.hpp"

#include <Maths/Transform.hpp>
#include <Scenes/Entity.hpp>

namespace test {
HeightDespawn::HeightDespawn(float removeHeight) :
	m_removeHeight(removeHeight) {
}

void HeightDespawn::Start() {
}

void HeightDespawn::Update() {
	auto transform = GetEntity()->GetComponent<Transform>();

	if (transform && transform->GetPosition().m_y < m_removeHeight) {
		GetEntity()->SetRemoved(true);
	}
}

const Node &operator>>(const Node &node, HeightDespawn &heightDespawn) {
	node["removeHeight"].Get(heightDespawn.m_removeHeight);
	return node;
}

Node &operator<<(Node &node, const HeightDespawn &heightDespawn) {
	node["removeHeight"].Set(heightDespawn.m_removeHeight);
	return node;
}
}
