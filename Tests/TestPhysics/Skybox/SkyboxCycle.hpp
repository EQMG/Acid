#pragma once

#include <Scenes/Component.hpp>
#include <Scenes/Entity.hpp>
#include <Files/Node.hpp>

using namespace acid;

namespace test {
class SkyboxCycle : public Component::Registrar<SkyboxCycle> {
	inline static const bool Registered = Register("skyboxCycle");
public:
	explicit SkyboxCycle(bool enableFog = true, bool enableRotation = false);

	void Start() override;
	void Update() override;

	friend const Node &operator>>(const Node &node, SkyboxCycle &skyboxCycle);
	friend Node &operator<<(Node &node, const SkyboxCycle &skyboxCycle);

private:
	bool enableFog;
	bool enableRotation;
};
}
