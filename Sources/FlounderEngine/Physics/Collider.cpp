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
		Update(*m_gameObject->GetTransform(), this);
	}
}
