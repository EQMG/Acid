#include "HeightDespawn.hpp"

#include <Maths/Transform.hpp>
#include <Scenes/Entity.inl>

namespace test {
bool HeightDespawn::registered = Register("heightDespawn");

HeightDespawn::HeightDespawn(float removeHeight) :
	m_removeHeight(removeHeight) {
}

void HeightDespawn::Update() {
	if (auto transform = GetEntity()->GetComponent<Transform>(); 
		transform && transform->GetPosition().m_y < m_removeHeight) {
		// TODO: Fix collider delete crash and crash from children not seeing this parent.
		//GetEntity()->SetRemoved(true);
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
