#pragma once

#include <Scenes/Component.hpp>
#include <Scenes/Entity.hpp>

using namespace acid;

namespace test
{
class SkyboxCycle :
	public Component
{
public:
	explicit SkyboxCycle(const bool &enableFog = true, const bool &enableRotation = false);

	void Start() override;

	void Update() override;

	friend const Metadata& operator>>(const Metadata& metadata, SkyboxCycle& skyboxCycle);

	friend Metadata& operator<<(Metadata& metadata, const SkyboxCycle& skyboxCycle);

private:
	bool m_enableFog;
	bool m_enableRotation;
};
}
