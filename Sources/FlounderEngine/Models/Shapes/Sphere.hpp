#pragma once

#include <functional>
#include "../Model.hpp"

namespace Flounder
{
	class Sphere :
		public Model
	{
	public:
		Sphere(const int &latitudeBands, const int &longitudeBands, const float &radius);

		~Sphere();
	};
}
