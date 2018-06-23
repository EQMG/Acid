#pragma once

#include "Engine/Exports.hpp"
#include "IComponent.hpp"

namespace fl
{
	class FL_EXPORT IBehaviour :
		public IComponent
	{
	private:
	public:
		IBehaviour() :
			IComponent()
		{
		}

		virtual ~IBehaviour()
		{
		}

		virtual void Update() override
		{
		}

		void Load(LoadedValue *value) override
		{
		}

		void Write(LoadedValue *destination) override
		{
		}

		std::string GetName() const override { return "Behaviour"; };
	};
}
