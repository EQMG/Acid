#include "HeightDespawn.hpp"

#include <Objects/GameObject.hpp>

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
		if (GetGameObject()->GetTransform().GetPosition().m_y < m_removeHeight)
		{
			GetGameObject()->SetRemoved(true);
		}
	}

	void HeightDespawn::Decode(const Metadata &metadata)
	{
	}

	void HeightDespawn::Encode(Metadata &metadata) const
	{
	}
}
