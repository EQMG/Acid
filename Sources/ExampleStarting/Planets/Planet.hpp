#pragma once

#include <Objects/Component.hpp>
#include <Objects/GameObject.hpp>

using namespace Flounder;

namespace Demo
{
	class F_EXPORT Planet :
		public Component
	{
	private:
		float m_apoapsis;
	public:
		Planet(const float &apoapsis = 0.0f);

		~Planet();

		void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Planet"; };
	};
}
