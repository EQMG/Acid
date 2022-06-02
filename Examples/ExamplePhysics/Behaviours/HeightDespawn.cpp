#include "HeightDespawn.hpp"

#include <Maths/Transform.hpp>
#include <Scenes/Entity.hpp>

namespace test {
HeightDespawn::HeightDespawn(float removeHeight) :
	removeHeight(removeHeight) {
}

void HeightDespawn::Start() {
}

void HeightDespawn::Update() {
	if (auto transform = GetEntity()->GetComponent<Transform>(); 
		transform && transform->GetPosition().y < removeHeight) {
		// TODO: Fix collider delete crash and crash from children not seeing this parent.
		//GetEntity()->SetRemoved(true);
	}
}

const Node &operator>>(const Node &node, HeightDespawn &heightDespawn) {
	node["removeHeight"].Get(heightDespawn.removeHeight);
	return node;
}

Node &operator<<(Node &node, const HeightDespawn &heightDespawn) {
	node["removeHeight"].Set(heightDespawn.removeHeight);
	return node;
}
}
