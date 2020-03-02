#pragma once

#include <Scenes/Component.hpp>

using namespace acid;

namespace test {
class PlayerFps : public Component::Registrar<PlayerFps> {
	inline static const bool Registered = Register("playerFps");
public:
	PlayerFps();

	void Start() override;
	void Update() override;

	friend const Node &operator>>(const Node &node, PlayerFps &player);
	friend Node &operator<<(Node &node, const PlayerFps &player);

private:
	bool noclipEnabled = false;
};
}
