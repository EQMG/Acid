#pragma once

#include "../maths/Vector2.hpp"
#include "../maths/Matrix4.hpp"

namespace Flounder
{
	struct UboScene
	{
		Matrix4 projection;
		Matrix4 view;
		Vector4 clip;
	};

	struct UboObject
	{
		Matrix4 model;
		float swaying;
		Vector2 swayOffset;
	};
}
