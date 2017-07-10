#pragma once

#include "../framework/framework.h"

namespace flounder {
	class shaders : public imodule
	{
	private:
	public:
		static shaders *get()
		{
			return (shaders*)framework::get()->getInstance("shaders");
		}

		shaders();

		~shaders();

		void init();

		void update();
	};
}
