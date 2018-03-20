#pragma once

#include <vector>
#include "../Maths/Vector2.hpp"
#include "../Maths/Vector3.hpp"
#include "../Engine/Platform.hpp"


namespace Flounder
{
	class F_EXPORT IVertex
	{
	public:
		IVertex();

		virtual ~IVertex();
	};
}