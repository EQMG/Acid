#pragma once

#include "../camera/icamera.hpp"
#include "../maths/vector4.hpp"
#include "../shaders/shader.hpp"
#include "../textures/texture.hpp"

#include "renderer.hpp"

namespace flounder
{
	/// <summary>
	/// Represents a sub renderer in the engine.
	/// </summary>
	class irenderer
	{
	public:
		/// <summary>
		/// Creates a new renderer.
		/// </summary>
		irenderer()
		{
		}

		/// <summary>
		/// Deconstructor for the renderer.
		/// </summary>
		virtual ~irenderer()
		{
		}

		/// <summary>
		/// Called when the renderer is needed to be rendered.
		/// </summary>
		/// <param name="clipPlane"> The current clip plane. </param>
		/// <param name="camera"> The camera to be used when rendering. </param>
		virtual void render(const vector4 &clipPlane, const icamera &camera) = 0;
	};
}
