#pragma once

#include <functional>
#include "Model.hpp"

namespace Flounder
{
	class CreateSphere
	{
	public:
		static Model *Create(const int &latitudeBands, const int &longitudeBands, const float &radius);
	};
}
