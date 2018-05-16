#pragma once

#include "Display/Display.hpp"
#include "Scenes/ICamera.hpp"
#include "Maths/Vector4.hpp"
#include "Buffers/CommandBuffer.hpp"

namespace fl
{
	/// <summary>
	/// Represents a sub renderer in the engine.
	/// </summary>
	class FL_EXPORT IRenderer
	{
	public:
		/// <summary>
		/// Creates a new renderer.
		/// </summary>
		IRenderer()
		{
		}

		/// <summary>
		/// Deconstructor for the renderer.
		/// </summary>
		virtual ~IRenderer()
		{
		}

		/// <summary>
		/// Called when the renderer is needed to be rendered.
		/// </summary>
		/// <param name="clipPlane"> The current clip plane. </param>
		/// <param name="camera"> The camera to be used when rendering. </param>
		virtual void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) = 0;
	};
}
