#include "Rotate.hpp"

//#include <bullet/LinearMath/btTransform.h>
#include <Scenes/Entity.hpp>
#include <Engine/Engine.hpp>
#include <Physics/Colliders/Collider.hpp>
#include <Maths/Maths.hpp>

namespace test
{
Rotate::Rotate(const Vector3f &direction, const int &test) :
	m_direction(direction),
	m_test(test)
{
}

void Rotate::Start()
{
}

void Rotate::Update()
{
	m_rotation += m_direction * Engine::Get()->GetDelta().AsSeconds();
	auto transform = GetEntity()->GetComponent<Transform>();

	if (!transform)
	{
		return;
	}

	transform->SetLocalRotation(m_rotation);

	if (m_test == 1)
	{
		Quaternion rotation(m_rotation);
		transform->SetLocalRotation(rotation.ToEuler());
	}
	else if (m_test == 2)
	{
		//auto transform1 = Collider::Convert(transform);
		//*transform = Collider::Convert(transform1, transform.GetScaling());
	}
}

const Node &operator>>(const Node &node, Rotate &rotate)
{
	node["Direction"].Get(rotate.m_direction);
	return node;
}

Node &operator<<(Node &node, const Rotate &rotate)
{
	node["Direction"].Set(rotate.m_direction);
	return node;
}
}
