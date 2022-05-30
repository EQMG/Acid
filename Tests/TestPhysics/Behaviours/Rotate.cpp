#include "Rotate.hpp"

//#include <bullet/LinearMath/btTransform.h>
#include <Scenes/Entity.hpp>
#include <Engine/Engine.hpp>
#include <Physics/Colliders/Collider.hpp>
#include <Maths/Maths.hpp>

namespace test {
Rotate::Rotate(const Vector3f &direction, const int &test) :
	direction(direction),
	test(test) {
}

void Rotate::Start() {
}

void Rotate::Update() {
	rotation += direction * Engine::Get()->GetDelta().AsSeconds();
	auto transform = GetEntity()->GetComponent<Transform>();
	if (!transform)
		return;

	transform->SetLocalRotation(rotation);

	if (test == 1) {
		Quaternion rotation(this->rotation);
		transform->SetLocalRotation(rotation.ToEuler());
	} else if (test == 2) {
		//auto transform1 = Collider::Convert(transform);
		//*transform = Collider::Convert(transform1, transform.GetScaling());
	}
}

const Node &operator>>(const Node &node, Rotate &rotate) {
	node["Direction"].Get(rotate.direction);
	return node;
}

Node &operator<<(Node &node, const Rotate &rotate) {
	node["Direction"].Set(rotate.direction);
	return node;
}
}
