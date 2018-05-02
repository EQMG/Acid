#pragma once

#include "Renderpass/RenderpassCreate.hpp"

namespace fl
{
	/// <summary>
	/// A extension used with IRenderer's to define a master renderer.
	/// </summary>
	class FL_EXPORT IManagerRender
	{
	public:
		/// <summary>
		/// Creates a new master renderer.
		/// </summary>
		IManagerRender(std::vector<RenderpassCreate *> renderpassCreate);

		/// <summary>
		/// Deconstructor for the master renderer.
		/// </summary>
		virtual ~IManagerRender();

		/// <summary>
		/// Run when rendering the master renderer.
		/// </summary>
		virtual void Render() = 0;
	};
}
