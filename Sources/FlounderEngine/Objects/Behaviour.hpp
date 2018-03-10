#pragma once

#include "../Prerequisites.hpp"
#include "Component.hpp"

namespace Flounder
{
	class F_EXPORT Behaviour :
		public Component
	{
	private:
	public:
		Behaviour();

		virtual ~Behaviour();

		virtual void Update() override;

		void Load(LoadedValue *value) override;

		void Write(LoadedValue *value) override;

		std::string GetName() const override { return "Behaviour"; };
	};
}
