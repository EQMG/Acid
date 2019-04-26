#include "HeightDespawn.hpp"

#include <Scenes/Entity.hpp>

namespace test
{
HeightDespawn::HeightDespawn(const float &removeHeight) :
	m_removeHeight(removeHeight)
{
}

void HeightDespawn::Start()
{
}

void HeightDespawn::Update()
{
	if (GetParent()->GetWorldTransform().GetPosition().m_y < m_removeHeight)
	{
		GetParent()->SetRemoved(true);
	}
}

const Metadata& operator>>(const Metadata& metadata, HeightDespawn& heightDespawn)
{
	metadata.GetChild("Remove Height", heightDespawn.m_removeHeight);
	return metadata;
}

Metadata& operator<<(Metadata& metadata, const HeightDespawn& heightDespawn)
{
	metadata.SetChild("Remove Height", heightDespawn.m_removeHeight);
	return metadata;
}
}
