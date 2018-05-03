#include "Collider.hpp"

namespace fl
{
	Collider::Collider() :
		Component()
	{
	}

	Collider::~Collider()
	{
	}

	void Collider::Update()
	{
		//	Update(*GetGameObject()->GetTransform(), this);
	}

	void Collider::Load(LoadedValue *value)
	{

	}

	void Collider::Write(LoadedValue *value)
	{

	}
}
