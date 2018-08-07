#pragma once

#include "Engine/Exports.hpp"
#include "IComponent.hpp"

namespace acid
{
	class ACID_EXPORT IBehaviour :
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

		virtual void Start() override
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
	};
}
