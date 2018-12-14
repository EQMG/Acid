#include "HeightDespawn.hpp"

#include <Objects/Entity.hpp>

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

	void HeightDespawn::Decode(const Metadata &metadata)
	{
		m_removeHeight = metadata.GetChild<float>("Remove Height");
	}

	void HeightDespawn::Encode(Metadata &metadata) const
	{
		metadata.SetChild<float>("Remove Height", m_removeHeight);
	}
}
