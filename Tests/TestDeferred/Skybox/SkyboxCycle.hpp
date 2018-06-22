#pragma once

#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>

using namespace fl;

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

		void Update() override;

		void Load(std::shared_ptr<LoadedValue> value) override;

		void Write(std::shared_ptr<LoadedValue> destination) override;

		std::string GetName() const override { return "SkyboxCycle"; };
	};
}