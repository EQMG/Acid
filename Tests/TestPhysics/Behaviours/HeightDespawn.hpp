#pragma once

#include <Scenes/Component.hpp>

using namespace acid;

namespace test
{
class HeightDespawn :
	public Component
{
public:
	explicit HeightDespawn(const float &removeHeight = -100.0f);

	void Start() override;

	void Update() override;

	friend const Metadata& operator>>(const Metadata& metadata, HeightDespawn& heightDespawn);

	friend Metadata& operator<<(Metadata& metadata, const HeightDespawn& heightDespawn);

private:
	float m_removeHeight;
};
}
