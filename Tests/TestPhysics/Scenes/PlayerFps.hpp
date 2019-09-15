#pragma once

#include <Scenes/Entity.hpp>
#include <Scenes/Component.hpp>
#include <Inputs/AxisCompound.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Physics/CollisionObject.hpp>

using namespace acid;

namespace test {
class PlayerFps : public Component {
public:
	PlayerFps();

	void Start() override;
	void Update() override;

	friend const Node &operator>>(const Node &node, PlayerFps &player);
	friend Node &operator<<(Node &node, const PlayerFps &player);

private:
	bool m_noclipEnabled = false;

	AxisCompound m_inputForward;
	AxisCompound m_inputStrafe;
	ButtonCompound m_inputSprint;
	ButtonCompound m_inputJump;
	ButtonCompound m_inputCrouch;
	ButtonCompound m_toggleNoclip;
};
}
