#pragma once

#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>

using namespace acid;

namespace test
{
	class SkyboxCycle :
		public IComponent
	{
	private:
		bool m_enableFog;
	public:
		SkyboxCycle(const bool &enableFog = true);

		~SkyboxCycle();

		void Start() override;

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *destination) override;
	};
}