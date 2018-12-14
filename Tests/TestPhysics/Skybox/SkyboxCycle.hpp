#pragma once

#include <Scenes/Component.hpp>
#include <Scenes/Entity.hpp>

using namespace acid;

namespace test
{
	class SkyboxCycle :
		public Component
	{
	private:
		bool m_enableFog;
		bool m_enableRotation;
	public:
		explicit SkyboxCycle(const bool &enableFog = true, const bool &enableRotation = false);

		void Start() override;

		void Update() override;

		void Decode(const Metadata &metadata) override;

		void Encode(Metadata &metadata) const override;
	};
}