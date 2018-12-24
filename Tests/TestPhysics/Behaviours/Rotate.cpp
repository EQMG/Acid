#include "Rotate.hpp"

//#include <bullet/LinearMath/btTransform.h>
#include <Scenes/Entity.hpp>
#include <Engine/Engine.hpp>
#include <Physics/Colliders/Collider.hpp>
#include <Maths/Maths.hpp>

namespace test
{
	Rotate::Rotate(const Vector3 &direction, const int &test) :
		m_direction(direction),
		m_test(test),
		m_rotation(Vector3())
	{
	}

	void Rotate::Start()
	{
	}

	void Rotate::Update()
	{
		m_rotation += m_direction * Engine::Get()->GetDelta().AsSeconds();
		Transform &transform = GetParent()->GetLocalTransform();
		transform.SetRotation(m_rotation);

		if (m_test == 1)
		{
			Quaternion rotation = Quaternion(m_rotation.m_x, m_rotation.m_y, m_rotation.m_z);
			transform.SetRotation(rotation.ToEuler());
		}
		else if (m_test == 2)
		{
		//	btTransform transform1 = Collider::Convert(transform);
		//	transform = Collider::Convert(transform1, transform.GetScaling());
		}
	}

	void Rotate::Decode(const Metadata &metadata)
	{
		m_direction = metadata.GetChild<Vector3>("Direction");
	}

	void Rotate::Encode(Metadata &metadata) const
	{
		metadata.SetChild<Vector3>("Direction", m_direction);
	}
}
