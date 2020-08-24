#pragma once

#include <Scenes/Component.hpp>

using namespace acid;

namespace test {
class FpsPlayer : public Component::Registrar<FpsPlayer> {
	static const bool Registered;
public:
	FpsPlayer();

	void Start() override;
	void Update() override;

	friend const Node &operator>>(const Node &node, FpsPlayer &player);
	friend Node &operator<<(Node &node, const FpsPlayer &player);

private:
	bool noclipEnabled = false;
};
}
