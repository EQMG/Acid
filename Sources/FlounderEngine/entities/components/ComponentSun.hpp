#pragma once

#include "../IComponent.hpp"

namespace Flounder
{
	class ComponentSun :
		public IComponent
	{
	private:
	public:
		ComponentSun();

		~ComponentSun();

		void Update() override;

		void CmdRender(EntityRender *entityRender) override;
	};
}
