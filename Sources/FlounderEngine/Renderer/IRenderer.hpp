#pragma once

#include "../Scenes/ICamera.hpp"
#include "../Display/Display.hpp"

namespace Flounder
{
	/// <summary>
	/// Represents a sub renderer in the engine.
	/// </summary>
	class F_EXPORT IRenderer
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
		virtual void Render(const VkCommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) = 0;
	};
}
