#include "HeightDespawn.hpp"

#include <Maths/Transform.hpp>
#include <Scenes/Entity.hpp>

namespace test
{
HeightDespawn::HeightDespawn(const float &removeHeight) :
	m_removeHeight{removeHeight}
{
}

void HeightDespawn::Start()
{
}

void HeightDespawn::Update()
{
	auto transform{GetEntity()->GetComponent<Transform>()};

	if (transform != nullptr && transform->GetPosition().m_y < m_removeHeight)
	{
		GetEntity()->SetRemoved(true);
	}
}

const Metadata &operator>>(const Metadata &metadata, HeightDespawn &heightDespawn)
{
	metadata.GetChild("removeHeight", heightDespawn.m_removeHeight);
	return metadata;
}

Metadata &operator<<(Metadata &metadata, const HeightDespawn &heightDespawn)
{
	metadata.SetChild("removeHeight", heightDespawn.m_removeHeight);
	return metadata;
}
}
