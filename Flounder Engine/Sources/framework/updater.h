#pragma once

#include <string>
#include "module.h"

namespace flounder {
	class updater {
	public:
		virtual void init() = 0;

		virtual void update() = 0;

		virtual module *getInstance(std::string name) = 0;
	};
}