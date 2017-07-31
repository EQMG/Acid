#pragma once

#include <functional>

#include "../devices/display.h"
#include "../maths/vector2.h"
#include "../maths/vector4.h"
#include "../maths/colour.h"
#include "../textures/texture.h"

namespace flounder
{
	class entity
	{
	private:
	public:
		entity();

		~entity();

		void update();
	};
}
