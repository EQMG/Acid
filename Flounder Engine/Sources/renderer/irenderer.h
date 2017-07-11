#pragma once

#include "../camera/icamera.h"
#include "../maths/vector4.h"
#include "../shaders/shader.h"
#include "../textures/texture.h"

#include "renderer.h"

namespace flounder {
	/// <summary>
	/// Represents a sub renderer in the engine.
	/// </summary>
	class irenderer
	{
	public:
		/// <summary>
		/// Called when the renderer is needed to be rendered.
		/// </summary>
		/// <param name="clipPlane"> The current clip plane. </param>
		/// <param name="camera"> The camera to be used when rendering. </param>
		virtual void render(vector4 *clipPlane, icamera *camera) = 0;
	};
}