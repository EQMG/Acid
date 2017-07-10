#pragma once

#include "../framework/framework.h"

namespace flounder {
	class models : public imodule
	{
	private:
	public:
		static models *get()
		{
			return (models*)framework::get()->getInstance("models");
		}

		models();

		~models();

		void init();

		void update();
	};
}
