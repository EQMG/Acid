#pragma once

#include <string>

namespace flounder {
	class model {
	public:
		model(const std::string &path)
		{

		}

		~model()
		{

		}

		int getVaoID() { return 1; }
		int getVaoLength() { return 1; }
	};
}