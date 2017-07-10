#pragma once

#include "../framework/framework.h"

namespace flounder {
	class textures : public imodule
	{
	private:
	public:
		static textures *get()
		{
			return (textures*)framework::get()->getInstance("textures");
		}

		textures();

		~textures();

		void init();

		void update();
	};
}
