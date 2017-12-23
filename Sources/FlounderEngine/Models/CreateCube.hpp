#pragma once

#include "Model.hpp"

namespace Flounder
{
	class CreateCube
	{
	public:
		static Model *Create(const float &width, const float &height, const float &depth);
	};
}
