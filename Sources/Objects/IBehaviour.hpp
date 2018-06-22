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

		void Load(std::shared_ptr<LoadedValue> value) override
		{
		}

		void Write(std::shared_ptr<LoadedValue> destination) override
		{
		}

		std::string GetName() const override { return "Behaviour"; };
	};
}
