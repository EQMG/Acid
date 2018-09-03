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

		void Decode(const Node &node) override
		{
		}

		void Encode(Node &node) const override
		{
		}
	};
}
