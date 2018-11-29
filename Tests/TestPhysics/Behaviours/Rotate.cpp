#include "Rotate.hpp"

#include <Objects/GameObject.hpp>
#include <Engine/Engine.hpp>
#include <Physics/Collider.hpp>
//#include <bullet/LinearMath/btTransform.h>
#include <Maths/Maths.hpp>

namespace test
{
	Rotate::Rotate(const Vector3 &direction, const bool &test) :
		m_direction(direction),
		m_test(test)
	{
	}

	void Rotate::Start()
	{
	}

	void Rotate::Update()
	{
		Transform &transform = GetGameObject()->GetTransform();
		transform.SetRotation(transform.GetRotation() + (m_direction * Engine::Get()->GetDelta().AsSeconds()));

		if (m_test)
		{
		//	btQuaternion test = btQuaternion(transform.GetRotation().m_y * DEG_TO_RAD, transform.GetRotation().m_x * DEG_TO_RAD, transform.GetRotation().m_z * DEG_TO_RAD);

		//	float pitch, yaw, roll;
		//	test.getEulerZYX(roll, yaw, pitch);
		//	Log::Out("%s -> (%f, %f, %f)\n", transform.GetRotation().ToString().c_str(), pitch * RAD_TO_DEG, yaw * RAD_TO_DEG, roll * RAD_TO_DEG);

		//	btTransform test = Collider::Convert(transform);

		//	float pitch, yaw, roll;
		//	test.getRotation().getEulerZYX(yaw, pitch, roll);
		//	Log::Out("%s -> (%f, %f, %f)\n", transform.GetRotation().ToString().c_str(), pitch, yaw, roll);

		//	Transform testNew = Collider::Convert(test, transform.GetScaling());
		//	transform.SetPosition(testNew.GetPosition());
		//	transform.SetRotation(testNew.GetRotation());
		//	transform.SetScaling(testNew.GetScaling());
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
