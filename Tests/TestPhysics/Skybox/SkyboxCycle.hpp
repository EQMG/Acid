#pragma once

#include <Scenes/Component.hpp>
#include <Files/Node.hpp>

using namespace acid;

namespace test {
class SkyboxCycle : public Component::Registrar<SkyboxCycle> {
public:
	explicit SkyboxCycle(bool enableFog = true, bool enableRotation = false);

	void Update() override;

	friend const Node &operator>>(const Node &node, SkyboxCycle &skyboxCycle);
	friend Node &operator<<(Node &node, const SkyboxCycle &skyboxCycle);

private:
	static bool registered;

	bool m_enableFog;
	bool m_enableRotation;
};
}
