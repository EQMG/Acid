#include "Collider.hpp"

namespace Flounder
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
