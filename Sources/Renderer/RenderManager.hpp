#pragma once

#include <map>
#include <memory>
#include "Renderpass/RenderpassCreate.hpp"

namespace acid
{
	class IRenderer;

	/// <summary>
	/// A object used to manage <seealso cref="IRenderer"/> objects to create a list of render pass.
	/// </summary>
	class ACID_EXPORT RenderManager
	{
	private:
		friend class Renderer;

		std::vector<RenderpassCreate> m_renderpassCreates;
		bool m_started;
	public:
		/// <summary>
		/// Creates a new render manager.
		/// </summary>
		/// <param name="renderpassCreates"> The renderpass pipeline to create for this manager. </param>
		explicit RenderManager(const std::vector<RenderpassCreate> &renderpassCreates) :
			m_renderpassCreates(renderpassCreates),
			m_started(false)
		{
		}

		/// <summary>
		/// Run when starting the renderer manager.
		/// </summary>
		virtual void Start() = 0;

		/// <summary>
		/// Run when updating the renderer manager.
		/// </summary>
		virtual void Update() = 0;
	};
}
