#pragma once

#include <functional>
#include "../Model.hpp"

namespace Flounder
{
	class Cube :
		public Model
	{
	public:
		Cube(const float &width, const float &height, const float &depth);
	};
}
