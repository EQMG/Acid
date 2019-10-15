#pragma once

#include <Scenes/Component.hpp>
#include <Files/Node.hpp>

using namespace acid;

namespace test {
class HeightDespawn : public Component::Registrar<HeightDespawn> {
public:
	explicit HeightDespawn(float removeHeight = -100.0f);

	void Update() override;

	friend const Node &operator>>(const Node &node, HeightDespawn &heightDespawn);
	friend Node &operator<<(Node &node, const HeightDespawn &heightDespawn);

private:
	static bool registered;

	float m_removeHeight;
};
}
