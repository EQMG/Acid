#pragma once

#include <vector>

#include "../framework/framework.h"

#include "istandard.h"

namespace flounder
{
	class standards : public imodule
	{
	private:
		std::vector<istandard*> *m_standards;
	public:
		static standards *get()
		{
			return (standards*)framework::get()->getInstance("standards");
		}

		standards();

		~standards();

		void addStandard(istandard *standard);

		void init();

		void update();
	};
}
